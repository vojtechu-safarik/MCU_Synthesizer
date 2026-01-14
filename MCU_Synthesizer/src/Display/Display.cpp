#include <Arduino.h>

#include "SPI.h"
#include "SD.h"

#include "ILI9341_t3n.h"
#include "ili9341_t3n_OCR_A_Custom_Font.h"

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
#include "Global_Variables\Wavetable_Variables.h"
// ==================

// === DISPLAY ===
#include "Display\Display.h"
// ==================

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
// ==================

// Create TFT Display object (SPI)
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST);

// ILI9341 commands
#define ILI9341_CASET 0x2A
#define ILI9341_PASET 0x2B
#define ILI9341_RAMWR 0x2C

// (global) -- display copies of waveforms (stable snapshot each refresh)
static int16_t displayWave_1[WAVE_LEN];
static int16_t displayWave_2[WAVE_LEN];
static int16_t displayWave_3[WAVE_LEN];
static int16_t displayWave_4[WAVE_LEN];
static int16_t displayWave_5[WAVE_LEN];
static int16_t displayWave_6[WAVE_LEN];

// refresh / throttling
const unsigned long REFRESH_MS = 33; // display main loop 30 Hz - only when Dirty 
unsigned long lastRefresh = 0;

// per-osc previous parameters (init values never happen)
int prevVolume[3] = { -1, -1, -1 };
int prevWaveFamily[3] = { -1, -1, -1 };
int prevShapeIndex[3] = { -1, -1, -1 };

// Layout
const uint16_t BG_COLOR = tft.color565(41, 41, 41);
const uint16_t FG_COLOR = ILI9341_WHITE;
const uint16_t ACCENT = tft.color565(128, 0, 0);

struct Rect { int x, y, w, h; };
Rect headerArea  = {0, 0, 320, 28};
Rect oscArea[3] = {
  {6, 32, 308, 64},
  {6, 100, 308, 64},
  {6, 168, 308, 64}
};
const int PAD = 6;

// buffer: 308x64 = 19712 px ~39 kB RAM
static uint16_t waveBuf[308 * 64];

// dirty flags and throttling for each OSC
bool oscDirty[6] = { true, true, true, true, true, true }; // initial true
bool valuesDirty[3] = { true, true, true };
unsigned long lastRedrawMs[6] = {0,0,0,0,0,0};
const unsigned long MIN_REDRAW_MS = 33; // min interval between each redraw for each OSC (ms)

// change detection globals
uint32_t prevWaveHash[6] = {0,0,0,0,0,0};
uint32_t currWaveHash[6] = {0,0,0,0,0,0};

// header change detection
byte prevSynthMode = 24;
int prevGlobalBPM = 999;

// header dirty flag
bool headerDirty = true;

// Helper: flush DMA cache
static inline void dmaFlush(void *ptr, int32_t bytes) {
  if (ptr == nullptr || bytes <= 0) return;
  uintptr_t addr = (uintptr_t)ptr;
  uintptr_t alignedAddr = addr & (~31u);
  int32_t alignedSize = (int32_t)((((addr + bytes + 31) & (~31u)) - alignedAddr));
  arm_dcache_flush((void*)alignedAddr, alignedSize);
}

// simple fast 32-bit FNV-1a hash for array int16_t (change detection)
uint32_t computeWaveHash(const int16_t *wave) {
  const uint32_t FNV_OFFSET = 2166136261u;
  const uint32_t FNV_PRIME  = 16777619u;
  uint32_t h = FNV_OFFSET;
  for (int i = 0; i < 256; ++i) {
    uint16_t v = (uint16_t)wave[i];
    // process by two byte (MSB, LSB)
    uint8_t b = (v >> 8) & 0xFF;
    h ^= b; h *= FNV_PRIME;
    b = v & 0xFF;
    h ^= b; h *= FNV_PRIME;
  }
  return h;
}

// Adaptive drawOscBlock_fast with sample-diff skipping
// Parameters
constexpr int SAMPLE_WIDTH = 180;        // how many px "columns" we compare
constexpr float THRESHOLD_AVG_DIFF = 8;  // treshold for avg abs diff

// save last samples, that have been drawn (for 3 OSC)
static int16_t prevSample[3][SAMPLE_WIDTH];

void safeTFTpinsInit() {
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  digitalWrite(TFT_CS, HIGH); // CS inactive
  digitalWrite(TFT_DC, LOW);  // DC safe default
}

// ========================================================
// ====== drawRightPanelAllOnce: Wave/Vol/Shape for 3 slots
void drawRightPanelAllOnce() {
  uint16_t *fb = tft.getFrameBuffer();
  if (!fb) return;

  const int GW = 200;
  int screenW = tft.width();

  // track bounding box for 3 right panels
  int bb_minx = INT_MAX, bb_miny = INT_MAX, bb_maxx = INT_MIN, bb_maxy = INT_MIN;

  for (int idx = 0; idx < 3; ++idx) {
    Rect r = oscArea[idx];
    int GH = r.h - 16; if (GH > 64) GH = 64;
    int gx = r.x + PAD;
    int gy = r.y + (r.h - GH) / 2;

    int tx = gx + GW + 16;
    int ty = gy;
    int areaW = r.w - (GW + PAD) - 8;
    int areaH = GH;

    // clear area can be made smaller from each side
    int clearX = tx;
    int clearY = ty - 1;            // keep 1px above edge
    int clearW = areaW + 6;
    int clearH = areaH + 2;

    // edge safety (edges are limited by the size of the screen)
    if (clearX < 0) { clearW -= (0 - clearX); clearX = 0; }
    if (clearY < 0) { clearH -= (0 - clearY); clearY = 0; }
    if (clearX + clearW > screenW) clearW = screenW - clearX;
    if (clearY + clearH > tft.height()) clearH = tft.height() - clearY;
    if (clearW <= 0 || clearH <= 0) continue;

    // draw into framebuffer (erase the text too)
    tft.fillRect(clearX, clearY, clearW, clearH, tft.color565(41, 41, 41));
    tft.setTextSize(1);
    tft.setTextColor(tft.color565(255, 191, 0));

    char tb[48];
    int waveFamily = (idx==0) ? currentWaveFamily_1 : (idx==1) ? currentWaveFamily_2 : currentWaveFamily_3;
    float vol = (idx==0) ? defaultVolume_1 : (idx==1) ? defaultVolume_2 : defaultVolume_3;
    int shapeIndex = (idx==0) ? currentShapeIndex_1 : (idx==1) ? currentShapeIndex_2 : currentShapeIndex_3;

    // 1) Wave – convert number to word (name)
    const char* waveNames[] = { "SAW", "SQUARE", "TRIANGLE", "SINE" };
    const char* waveName = (waveFamily >= 0 && waveFamily < 4) ? waveNames[waveFamily] : "?";
    snprintf(tb, sizeof(tb), waveName);
    tft.setCursor(tx, ty);
    tft.print(tb);

    // 2) Volume – normalise displayed volume to 0–99
    int volNorm = int(vol * 132.0);
    snprintf(tb, sizeof(tb), "Vol: %d", volNorm);
    tft.setCursor(tx, ty + 16);
    tft.print(tb);

    // 3) Shape – normalise displayed shape to 0–99 according to its max
    int shapeNorm = shapeIndex;
    if (waveFamily == 0) {
      shapeNorm = int(shapeIndex * 0.99);
    } else if (waveFamily == 1) {
      shapeNorm = int(shapeIndex * 5.24);
    } else if (waveFamily == 2) {
      shapeNorm = int(shapeIndex * 0.89);
    } else if (waveFamily == 3) {
      shapeNorm = int(shapeIndex * 1.38);
    }

    snprintf(tb, sizeof(tb), "Shape: %d", shapeNorm);
    tft.setCursor(tx, ty + 32);
    tft.print(tb);

    // increase the size of bounding box by the size of clear area
    int local_minx = clearX;
    int local_miny = clearY;
    int local_maxx = clearX + clearW - 1;
    int local_maxy = clearY + clearH - 1;

    if (local_minx < bb_minx) bb_minx = local_minx;
    if (local_miny < bb_miny) bb_miny = local_miny;
    if (local_maxx > bb_maxx) bb_maxx = local_maxx;
    if (local_maxy > bb_maxy) bb_maxy = local_maxy;
  }

  // if no change, return
  if (bb_minx == INT_MAX) return;

  // set to correct size for DMA flush inside dmaFlush
  int regionW = bb_maxx - bb_minx + 1;
  int regionH = bb_maxy - bb_miny + 1;
  uint16_t *startPtr = fb + (bb_miny * screenW + bb_minx);

  // flush cache and oneshot async update of the entire bounding area
  dmaFlush(startPtr, regionW * regionH * 2);
  tft.setClipRect(bb_minx, bb_miny, regionW, regionH);
  if (!tft.updateScreenAsync(false)) {
    // if driver busy, skip and try again next tick
  }
    for (int i = 0; i < 3; i++) {
        valuesDirty[i] = false;
    }
}

void drawStaticLayout() {
  tft.fillScreen(BG_COLOR);

  // header background
  tft.fillRect(headerArea.x, headerArea.y, headerArea.w, headerArea.h, ILI9341_RED);

  // box parameters (GW consistent with drawOscBlock_fast)
  const int GW = 200;
  int leftBoxW = GW + PAD*2 + 8; // width of the left box for the waveform

  // box colors
  uint16_t boxBg = ILI9341_BLACK;
  uint16_t borderLine = tft.color565(110, 0, 0);

  // draw the left boxes for the waveforms
  for (int i = 0; i < 3; ++i) {
    Rect r = oscArea[i];
    int bx = r.x;
    int by = r.y;
    int bw = leftBoxW;
    int bh = r.h;

    // fill the box background
    tft.fillRoundRect(bx, by, bw, bh, 6, boxBg);

    // draw the border line around the left box
    tft.drawRoundRect(bx-1, by-1, bw+2, bh+2, 6, borderLine);
  }
  headerDirty = true;
}

// ======================================================================================
// ============ check for changes in wavetables / header and set dirty flags ============
void checkForChangesAndSetDirty() {
  // base = 0 (1..3) or 3 (4..6) waveforms are being shown (switch in Unison SynthMode depending on unisonTriad)
  int srcBase;
  if (((SynthMode == 0) && (unisonTriad == 0)) || (SynthMode == 1)) srcBase = 0;
  else srcBase = 3;

  for (int i = 0; i < 3; ++i) {
    int src = srcBase + i; // index for displayWave_1..6
    // set pointer for the correct array for hash-check
    const int16_t *wavePtr = nullptr;
    if (src == 0) wavePtr = displayWave_1;
    else if (src == 1) wavePtr = displayWave_2;
    else if (src == 2) wavePtr = displayWave_3;
    else if (src == 3) wavePtr = displayWave_4;
    else if (src == 4) wavePtr = displayWave_5;
    else if (src == 5) wavePtr = displayWave_6;

    uint32_t h = computeWaveHash(wavePtr);
    if (h != prevWaveHash[src]) {
      oscDirty[i] = true;           // set dirty for drawn slot i
      prevWaveHash[src] = h;
    }

    // per-slot params are mapped only for one triad, group 1 .. 3  (parameters are shared for both triads)
      float curVolF = (i == 0) ? defaultVolume_1 : (i == 1) ? defaultVolume_2 : defaultVolume_3;
      int curVolume = (int)floorf(curVolF * 127.0f + 0.5f);

      int curFamily = (i == 0) ? currentWaveFamily_1 : (i == 1) ? currentWaveFamily_2 : currentWaveFamily_3;
      int curShape  = (i == 0) ? currentShapeIndex_1  : (i == 1) ? currentShapeIndex_2  : currentShapeIndex_3;

      if (curVolume != prevVolume[i] ||
          curFamily != prevWaveFamily[i] ||
          curShape  != prevShapeIndex[i]) {
        // mark dirty for this visible slot
        valuesDirty[i] = true;

        // update cached values
        prevVolume[i] = curVolume;
        prevWaveFamily[i] = curFamily;
        prevShapeIndex[i] = curShape;
      }
  }

  // header params (if changed) — set headerDirty
  if (SynthMode != prevSynthMode || GlobalBPM != prevGlobalBPM) {
    prevSynthMode = SynthMode;
    prevGlobalBPM = GlobalBPM;
    headerDirty = true;
  }
}

// redraws changes in Header
void drawParams() {
  if (!headerDirty) return;

  tft.fillRect(headerArea.x, headerArea.y, headerArea.w, headerArea.h, ILI9341_RED);

  const char *modeText = (SynthMode == 0) ? "UNISON" : "POLY";
  char bufMode[48];
  snprintf(bufMode, sizeof(bufMode), "Synth Mode: %s", modeText);

  char bufBPM[32];
  snprintf(bufBPM, sizeof(bufBPM), "BPM: %d", GlobalBPM);

  tft.setTextSize(1);
  
  tft.setTextColor(tft.color565(255, 191, 0), ILI9341_RED);

  // SynthMode left
  tft.setCursor(headerArea.x + 6, headerArea.y + 4);
  tft.print(bufMode);

  // BPM right
  int approxCharW = 6;
  int wBPM = strlen(bufBPM) * approxCharW;
  int bpmX = headerArea.x + headerArea.w - 42 - wBPM;
  tft.setCursor(bpmX, headerArea.y + 4);
  tft.print(bufBPM);

  // flush and DMA update
  uint16_t *fb = tft.getFrameBuffer();
  if (fb != nullptr) {
    int screenW = tft.width();
    uint16_t *startPtr = fb + (headerArea.y * screenW + headerArea.x);
    dmaFlush(startPtr, headerArea.w * headerArea.h * 2);
    tft.setClipRect(headerArea.x, headerArea.y, headerArea.w, headerArea.h);
    bool started = tft.updateScreenAsync(false);
    headerDirty = !started;
  } else {
    headerDirty = false;
  }
}

// ============ Fast redraw of the whole waveform area ============
// minimise number of SPI transactions -> less audio interrupts and less display blinking

void drawOscBlock_fast(int idx) {
  if (idx < 0 || idx > 2) return;

  unsigned long now = millis();
 // --- ensure parameter-changes (volume/family/shape) also force redraw if missed ---
  // read "per-slot" parameters (mapped as a group 1..3)
  float curVolF = (idx == 0) ? defaultVolume_1 : (idx == 1) ? defaultVolume_2 : defaultVolume_3;
  int curVolume = (int)floorf(curVolF * 127.0f + 0.5f);
  int curFamily = (idx == 0) ? currentWaveFamily_1 : (idx == 1) ? currentWaveFamily_2 : currentWaveFamily_3;
  int curShape  = (idx == 0) ? currentShapeIndex_1  : (idx == 1) ? currentShapeIndex_2  : currentShapeIndex_3;

  // local volume tracking
  static float prevVolF_local[3] = { -1.0f, -1.0f, -1.0f };
  const float VOL_EPS = 0.002f;

  if (fabs(curVolF - prevVolF_local[idx]) > VOL_EPS ||
      curFamily != prevWaveFamily[idx] ||
      curShape  != prevShapeIndex[idx]) {
    oscDirty[idx] = true;
    prevVolF_local[idx] = curVolF;
    prevVolume[idx] = curVolume;
    prevWaveFamily[idx] = curFamily;
    prevShapeIndex[idx] = curShape;
    lastRedrawMs[idx] = 0;
  }

  if (!oscDirty[idx] && now - lastRedrawMs[idx] < MIN_REDRAW_MS) return;
  if (!oscDirty[idx]) return;

  Rect r = oscArea[idx];

  // waveform area constants
  constexpr int GW = 200;
  int GH = r.h - 16; if (GH > 64) GH = 64;
  int gx = r.x + PAD;
  int gy = r.y + (r.h - GH) / 2;

  // per-slot wave pointer and waveform color (yellow)
  int16_t *wave = nullptr;
  uint16_t waveColor = tft.color565(255, 191, 0);
  float vol = 0.0f;
  int waveFamily = 0, shapeIndex = 0;
  if (((SynthMode == 0) && (unisonTriad == 0)) || (SynthMode == 1)) {
    if (idx == 0) { wave = displayWave_1; waveFamily = currentWaveFamily_1; shapeIndex = currentShapeIndex_1; vol = defaultVolume_1; }
    else if (idx == 1) { wave = displayWave_2; waveFamily = currentWaveFamily_2; shapeIndex = currentShapeIndex_2; vol = defaultVolume_2; }
    else if (idx == 2) { wave = displayWave_3; waveFamily = currentWaveFamily_3; shapeIndex = currentShapeIndex_3; vol = defaultVolume_3; }
  } else {
    if (idx == 0) { wave = displayWave_4; waveFamily = currentWaveFamily_1; shapeIndex = currentShapeIndex_1; vol = defaultVolume_1; }
    else if (idx == 1) { wave = displayWave_5; waveFamily = currentWaveFamily_2; shapeIndex = currentShapeIndex_2; vol = defaultVolume_2; }
    else if (idx == 2) { wave = displayWave_6; waveFamily = currentWaveFamily_3; shapeIndex = currentShapeIndex_3; vol = defaultVolume_3; }
  }
  if (wave == nullptr) { oscDirty[idx] = false; return; }

  // --- 1) quick sample-diff check ---
  int compareW = min(GW, SAMPLE_WIDTH);
  long sumAbs = 0;
  for (int x = 0; x < compareW; ++x) {
    int idxf = (int)((uint32_t)x * (WAVE_LEN - 1) / (compareW - 1));
    int16_t s = wave[idxf];
    int16_t prev = prevSample[idx][x];
    int diff = (int)s - (int)prev;
    if (diff < 0) diff = -diff;
    sumAbs += diff;
  }
  float avgDiff = (float)sumAbs / (float)compareW;
  if (avgDiff < THRESHOLD_AVG_DIFF && (now - lastRedrawMs[idx] < MIN_REDRAW_MS*2)) {
    oscDirty[idx] = false;
    return;
  }

  // --- 2) update prevSample ---
  for (int x = 0; x < compareW; ++x) {
    int idxf = (int)((uint32_t)x * (WAVE_LEN - 1) / (compareW - 1));
    prevSample[idx][x] = wave[idxf];
  }

  // --- 3) render into local waveBuf (bitmap for waveform) ---
  int w = GW;
  int h = GH;
  if (w > 308) w = 308;
  int total = w * h;

  // box background
  uint16_t boxBg = ILI9341_BLACK;
  memset(waveBuf, 0, total * sizeof(uint16_t));
  // fill the box background
  for (int i = 0; i < total; ++i) waveBuf[i] = boxBg;

  // thicker wave
  const int WAVE_THICKNESS = 1; // can be changed, 1 - 4 is usable
  for (int x = 0; x < w; ++x) {
    int idxf = (int)((uint32_t)x * (WAVE_LEN - 1) / (w - 1));
    if (idxf < 0) idxf = 0; if (idxf >= WAVE_LEN) idxf = WAVE_LEN - 1;
    int32_t samp = (int32_t)wave[idxf];
    int32_t scaled = (samp * ((h-1) / 2)) / 32768;  // "h-1" adjusts the height of the waveform so that it isnt covered by the box line
    int y = ((h-1) / 2) - scaled;                   // "h-1" - the same thing
    if (y < 0) y = 0; if (y >= h) y = h - 1;
    // few pixels vertically around y
    for (int t = -WAVE_THICKNESS; t <= WAVE_THICKNESS; ++t) {
      int yy = y + t;
      if (yy < 0 || yy >= h) continue;
      waveBuf[yy * w + x] = waveColor;
    }
  }

  // --- 4) copy waveBuf into internal framebuffer (row by row) ---
  uint16_t *fb = tft.getFrameBuffer();
  if (fb == nullptr) {
    // fallback: draw into screen directly and return (not recommended)
    tft.setClipRect(gx, gy, w, h);
    tft.setFrameBuffer((uint16_t*)waveBuf);
    tft.updateScreen();
    oscDirty[idx] = false;
    lastRedrawMs[idx] = millis();
    return;
  }

  int screenW = tft.width();
  for (int y = 0; y < h; ++y) {
    uint16_t *dest = fb + ((gy + y) * screenW + gx);
    uint16_t *src  = waveBuf + (y * w);
    memcpy(dest, src, w * sizeof(uint16_t));
  }

  // --- 5) draw right-side text INTO FRAMEBUFFER (before DMA) ---
  int tx = gx + GW + 16;
  int ty = gy;
  int areaW = r.w - (GW + PAD) - 8;
  int areaH = GH;

  // erase text area directly in interal frambuffer
  tft.fillRect(tx, ty - 2, areaW + 8, areaH + 4, tft.color565(41, 41, 41));

  // draw texts (small font)
  tft.setTextSize(1);
  tft.setTextColor(tft.color565(255, 191, 0), tft.color565(41, 41, 41));
  char tb[48];

  // 1) Wave – convert number to name
  const char* waveNames[] = { "SAW", "SQUARE", "TRIANGLE", "SINE" };
  const char* waveName = (waveFamily >= 0 && waveFamily < 4) ? waveNames[waveFamily] : "?";
  snprintf(tb, sizeof(tb), waveName);
  tft.setCursor(tx, ty);
  tft.print(tb);

  // 2) Volume – normalise to 0–99
  int volNorm = int(vol * 132.0);
  snprintf(tb, sizeof(tb), "Vol: %d", volNorm);
  tft.setCursor(tx, ty + 16);
  tft.print(tb);

  // 3) Shape – normalise to 0–99 according to its specific maximum
  int shapeNorm = shapeIndex;
  if (waveFamily == 0) {
    shapeNorm = int(shapeIndex * 0.99);
  } else if (waveFamily == 1) {
    shapeNorm = int(shapeIndex * 5.24);
  } else if (waveFamily == 2) {
    shapeNorm = int(shapeIndex * 0.89);
  } else if (waveFamily == 3) {
    shapeNorm = int(shapeIndex * 1.38);
  }
  
  snprintf(tb, sizeof(tb), "Shape: %d", shapeNorm);
  tft.setCursor(tx, ty + 32);
  tft.print(tb);

  // --- 6) flush combined region (waveform + right panel) and start async update ---
  int minX = gx;
  int maxX = r.x + r.w - 1; // right area corner
  int regionW = maxX - minX + 1;
  int32_t bytes = regionW * h * 2; // bytes to flush
  uint16_t *startPtr = fb + (gy * screenW + minX);
  dmaFlush(startPtr, bytes);

  // set clip to whole area (wave + right text) and start DMA oneshot
  tft.setClipRect(minX, gy, regionW, h);
  if (!tft.updateScreenAsync(false)) {
    // if busy, mark dirty so that it is repeated later
    oscDirty[idx] = true;
    valuesDirty[idx] = true;
  } else {
    // success start -> clear dirty & set timestamp
    oscDirty[idx] = false;
    valuesDirty[idx] = false;
    lastRedrawMs[idx] = millis();
  }
}

// =====================================================================
// ============================= S E T U P =============================
// =====================================================================
void Display_setup() {
  safeTFTpinsInit();

  SPI.begin();
  tft.begin();
  tft.useFrameBuffer(true);
  // tft.setClock(30000000); // Default value, setClock might not work as is with the library ILI9341_t3n
  tft.setRotation(1); // landscape (rotate 90°) -> width 320, height 240
  tft.fillScreen(BG_COLOR);
  tft.setTextColor(FG_COLOR); // VYZKOUŠET - TEĎ JE WHITE, NENÍ TO PŘÍČINA BUGU?
  tft.setTextSize(1);
  tft.setFont(OCR_A_Custom_Font_10);

  // initial snapshot of current waveforms for display rendering
  Display_Waveform_Snapshot();

  // init prev per-osc values
  prevVolume[0]      = defaultVolume_1;
  prevVolume[1]      = defaultVolume_2;
  prevVolume[2]      = defaultVolume_3;

  prevWaveFamily[0]  = currentWaveFamily_1;
  prevWaveFamily[1]  = currentWaveFamily_2;
  prevWaveFamily[2]  = currentWaveFamily_3;

  prevShapeIndex[0]  = currentShapeIndex_1;
  prevShapeIndex[1]  = currentShapeIndex_2;
  prevShapeIndex[2]  = currentShapeIndex_3;

  prevSynthMode = SynthMode;
  prevGlobalBPM = GlobalBPM;

  drawStaticLayout();
  drawRightPanelAllOnce();
}

void Display_Waveform_Snapshot() {
  memcpy(displayWave_1, myWaveform_1, WAVE_LEN * sizeof(int16_t));
  memcpy(displayWave_2, myWaveform_2, WAVE_LEN * sizeof(int16_t));
  memcpy(displayWave_3, myWaveform_3, WAVE_LEN * sizeof(int16_t));
  
  if ((SynthMode == 0) && (LFOtypeSelect == 1)) {
  memcpy(displayWave_4, myWaveform_4, WAVE_LEN * sizeof(int16_t));
  memcpy(displayWave_5, myWaveform_5, WAVE_LEN * sizeof(int16_t));
  memcpy(displayWave_6, myWaveform_6, WAVE_LEN * sizeof(int16_t));  
  } else {
  memcpy(displayWave_4, myWaveform_1, WAVE_LEN * sizeof(int16_t));
  memcpy(displayWave_5, myWaveform_2, WAVE_LEN * sizeof(int16_t));
  memcpy(displayWave_6, myWaveform_3, WAVE_LEN * sizeof(int16_t));  
  }  
}

// =====================================================================
// ============================== L O O P ==============================
// =====================================================================
void Display_loop() {
  unsigned long nowDisplay = millis();
  if (nowDisplay - lastRefresh >= REFRESH_MS) {
    lastRefresh = nowDisplay;

    // Update display waveform snapshot
    Display_Waveform_Snapshot(); 

    checkForChangesAndSetDirty();


    // draw each block only if dirty
    for (int i = 0; i < 3; i++) {
        drawOscBlock_fast(i);
    }

    // if text values are dirty, redraw them all at once
    if (valuesDirty[0] || valuesDirty[1] || valuesDirty[2]) {
        drawRightPanelAllOnce(); // this function then erases the dirty flags
    }

    // update text values and draw the waveform for each OSC array
    drawParams();
  }
}