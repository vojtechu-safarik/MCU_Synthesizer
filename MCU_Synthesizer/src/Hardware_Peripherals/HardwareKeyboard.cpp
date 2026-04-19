#include <Arduino.h>

// === KEYPAD lib ===
#include "Keypad.h"
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/MIDI_Config.h"
#include "Configuration/Pins_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/HardwareKeyboard.h"
// ==================

/* === CONFIGURATION === */

/* SeqMode: 
    0 = Off
    1 = Arp
    2 = Latch
SeqOrder: 
    0 = Up
    1 = Down
    2 = Queue
SeqOctaves: 
    0 = 1 oct
    1 = 2 oct
    2 = 3 oct
SeqGatePot: 
    1 .. 100 
    ( how big portion of the step is the note in an active state )
SeqRateSelect:
    0 = 1/2 step length (BPM)
    1 = 1/4 step length (BPM)
    2 = 1/8 step length (BPM)
    3 = 1/16 step length (BPM)
*/

/* ===================== */

static const byte ROWS = 9;
static const byte COLS = 5;
static byte rowPins[ROWS] = {Row_1, Row_2, Row_3, Row_4, Row_5, Row_6, Row_7, Row_8, Row_9};
static byte colPins[COLS] = {Column_1, Column_2, Column_3, Column_4, Column_5};

static char keymap[ROWS][COLS] = {
  { 1,  2,  3,  4,  5},
  { 6,  7,  8,  9, 10},
  {11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20},
  {21, 22, 23, 24, 25},
  {26, 27, 28, 29, 30}, 
  {31, 32, 33, 34, 35},
  {36, 37, 38, 39, 40},
  {41, 42, 43, 44, 45}
};

// MIDI notes: 254 = Rest (pause)
static const byte keyToMidiNote[45] = {
  // === Row 1 (Blue 1) ===
  0,   // [0] Column 1 (R1 B1): Oct Down
  44,  // [1] Column 2 (R2 B1): Note E2
  0,   // [2] Column 3 (R3 B1): (Shift)
  46,  // [3] Column 4 (R4 B1): Note F#2
  56,  // [4] Column 5 (R5 B1): Note E3

  // === Row 2 (Blue 2) ===
  0,   // [5] Column 1 (R1 B2): Oct Up
  42,  // [6] Column 2 (R2 B2): Note D2
  0,   // [7] Column 3 (R3 B2): (LFO - Rise / Fall)
  48,  // [8] Column 4 (R4 B2): Note G#2
  55,   // [9] Column 5 (R5 B2): Note D#3

  // === Row 3 (Blue 3) ===
  28,  // [10] Column 1 (R1 B3): Note C1 (First Note)
  40,  // [11] Column 2 (R2 B3): Note C2
  0,   // [12] Column 3 (R3 B3): (LFO - Free / Trig)
  50,  // [13] Column 4 (R4 B3): Note A#2
  53,  // [14] Column 5 (R5 B3): Note C#3

  // === Row 4 (Blue 4) ===
  30,  // [15] Column 1 (R1 B4): Note D1
  39,  // [16] Column 2 (R2 B4): Note B1
  0,  // [17] Column 3 (R3 B4): Drone #1
  45,  // [18] Column 4 (R4 B4): Note F2
  0,   // [19] Column 5 (R5 B4): Drone #5

  // === Row 5 (Blue 5) ===
  32,  // [20] Column 1 (R1 B5): Note E1
  37,  // [21] Column 2 (R2 B5): Note A1
  0,  // [22] Column 3 (R3 B5): Drone #2
  47,  // [23] Column 4 (R4 B5): Note G2
  0,   // [24] Column 5 (R5 B5): Drone #6

  // === Row 6 (Blue 6) ===
  33,  // [25] Column 1 (R1 B6): Note F1
  35,  // [26] Column 2 (R2 B6): Note G1
  0,  // [27] Column 3 (R3 B6): Drone #3
  49,  // [28] Column 4 (R4 B6): Note A2
  0,   // [29] Column 5 (R5 B6): Drone #7

  // === Row 7 (Blue 7) ===
  0,   // [30] Column 1 (R1 B7): (Unused)
  38, // [31] Column 2 (R2 B7): Note A#1
  0,  // [32] Column 3 (R3 B7): Drone #4
  51,  // [33] Column 4 (R4 B7): Note B2
  0,   // [34] Column 5 (R5 B7): Drone #8

  // === Row 8 (Blue 8) ===
  29,  // [35] Column 1 (R1 B8): Note C#1
  36,   // [36] Column 2 (R2 B8): Note G#1
  41,  // [37] Column 3 (R3 B8): Note C#2
  52,  // [38] Column 4 (R4 B8): Note C3
  0,   // [39] Column 5 (R5 B8): (Unused)

  // === Row 9 (Blue 9) ===
  31,  // [40] Column 1 (R1 B9): Note D#1
  34,   // [41] Column 2 (R2 B9): Note F#1
  43,  // [42] Column 3 (R3 B9): Note D#2
  54,  // [43] Column 4 (R4 B9): Note D3
  0    // [44] Column 5 (R5 B9): (Unused)
};

static const byte keyToLFOWave[45] = {
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,
    0, 255, 255, 255, 255,
    1, 255, 255, 255, 255,
    2, 255, 255, 255, 255,
    3,   4, 255, 255, 255,
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255
};

static const byte keyToPORTlockRate[45] = {
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,
  255,   4, 255, 255, 255,
    0,   3, 255, 255, 255,
    1,   2, 255, 255, 255
};

static constexpr byte HW_CHANNEL  = 0;
static constexpr byte HW_VELOCITY = 127;

static constexpr byte KEY_OCT_DOWN   = 0;
static constexpr byte KEY_OCT_UP     = 5;
static constexpr byte KEY_SHIFT      = 2;

/* ================== STATE VARIABLES ================== */

static Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

static byte activeKeyStack[32];  // Amount of notes in Latch
static byte activeKeyCount = 0;
static int playingNotes[45];     // Tracking the notes in Direct Play for immediate transposition

int sequenceBuffer[256];
byte sequenceLength = 0;

static unsigned long internal_lastStepTime = 0;
static int internal_currentlyPlayingNote = -1;
static unsigned long internal_gateOffTime = 0;
static byte internal_playHead = 0; 

static bool shiftActive = false;
// restActive state variables removed

int octaveValue = 2; 

/* ================== HELPERS ================== */

void internal_stopNote() {
    if (internal_currentlyPlayingNote >= 0) {
        myNoteOff(HW_CHANNEL, (byte)internal_currentlyPlayingNote, 0);
        internal_currentlyPlayingNote = -1;
    }
}

// Immediately change the pitch of Direct play
void updateDirectPlayOctave() {
    if (CurrentSeqMode != 0) return; 
    // Loop through all possible keys instead of bounded notes
    for (int i = 0; i < 45; i++) {
        if (playingNotes[i] != -1) {
            myNoteOff(HW_CHANNEL, (byte)playingNotes[i], 0);
            int newNote = keyToMidiNote[i] + ((octaveValue - 2) * 12);
            if (newNote < 0) newNote = 0; if (newNote > 127) newNote = 127;
            playingNotes[i] = newNote;
            myNoteOn(HW_CHANNEL, (byte)newNote, HW_VELOCITY);
        }
    }
}

void pushKey(byte keyIndex) {
    // Arp mode (1) doesnt support duplicities
    if (CurrentSeqMode == 1) {
        for (byte i = 0; i < activeKeyCount; i++) {
            if (activeKeyStack[i] == keyIndex) return; 
        }
    }
    
    // Latch mode (2) can have the same notes in a row
    if (activeKeyCount < 12) {
        activeKeyStack[activeKeyCount++] = keyIndex;
    }
}

void popKey(byte keyIndex) {
    bool found = false;
    for (byte i = 0; i < activeKeyCount; i++) {
        if (activeKeyStack[i] == keyIndex) found = true;
        if (found && i < 11) activeKeyStack[i] = activeKeyStack[i+1];
    }
    if (found && activeKeyCount > 0) activeKeyCount--;
}

static unsigned long shiftPressTime = 0;
static bool latchCleared = false;

/* ================== SEQUENCER LOGIC ================== */

void rebuildSequence() {
    sequenceLength = 0;
    if (activeKeyCount == 0) return;

    int tempNotes[12];
    for (byte i = 0; i < activeKeyCount; i++) {
        // We handle 254 as a REST natively in the array now
        tempNotes[i] = keyToMidiNote[activeKeyStack[i]];
    }

    // Sort for UP/DOWN (Queue mode 2 is being skipped)
    if (CurrentSeqOrder < 2) {
        for (byte i = 0; i < activeKeyCount; i++) {
            for (byte j = i + 1; j < activeKeyCount; j++) {
                if (tempNotes[j] < tempNotes[i]) {
                    int t = tempNotes[i]; tempNotes[i] = tempNotes[j]; tempNotes[j] = t;
                }
            }
        }
        if (CurrentSeqOrder == 1) { // Down
            for (byte i = 0; i < activeKeyCount / 2; i++) {
                int t = tempNotes[i];
                tempNotes[i] = tempNotes[activeKeyCount - 1 - i];
                tempNotes[activeKeyCount - 1 - i] = t;
            }
        }
    }

    // Expansion + Final Buffer
    for (byte i = 0; i < activeKeyCount; i++) {
        int base = tempNotes[i];
        for (byte r = 0; r <= CurrentSeqOctave; r++) {
            if (sequenceLength < 32) {
                // If pause (254), dont increment octaves
                sequenceBuffer[sequenceLength++] = (base == 254) ? 254 : (base + (r * 12));
            }
        }
    }
}

void internal_sequencerUpdate() {
    if (CurrentSeqMode == 0) return;

    unsigned long now = millis();

    if (internal_currentlyPlayingNote >= 0 && internal_gateOffTime > 0 && now >= internal_gateOffTime) {
        internal_stopNote();
        internal_gateOffTime = 0;
    }

    int bpm = (GlobalBPM < 1) ? 120 : GlobalBPM;
    unsigned long stepMs = 60000UL / bpm; 

    if (SeqRateSelect == 0) {
        stepMs = stepMs * 2;
    } else if (SeqRateSelect == 2) {
        stepMs = stepMs / 2;
    } else if (SeqRateSelect == 3) {
        stepMs = stepMs / 4;
    }

    if ((now - internal_lastStepTime) < stepMs) return;
    internal_lastStepTime = now;

    rebuildSequence();
    if (sequenceLength == 0) {
        internal_playHead = 0;
        return;
    }

    if (internal_playHead >= sequenceLength) internal_playHead = 0;

    int rawNote = sequenceBuffer[internal_playHead];
    internal_stopNote();

    if (rawNote == 254) {
        internal_currentlyPlayingNote = -1; // pause
    } else {
        int finalNote = rawNote + ((octaveValue - 2) * 12);
        if (finalNote < 0) finalNote = 0; if (finalNote > 127) finalNote = 127;
        internal_currentlyPlayingNote = finalNote;
        myNoteOn(HW_CHANNEL, (byte)finalNote, HW_VELOCITY);
    }

    internal_gateOffTime = now + (stepMs * SeqGatePot / 100);
    internal_playHead++;
}

/* ================== CORE LOOP ================== */

void Keyboard_init() {
    for(int i=0; i<45; i++) playingNotes[i] = -1;
    activeKeyCount = 0;
    internal_lastStepTime = millis();
}

void Keyboard_update() {
    // 1. Sequencer must run all the time
    internal_sequencerUpdate();

    // Sequence deletion logic temporarily removed
    if (shiftActive && !latchCleared && (millis() - shiftPressTime > 3000)) {
        activeKeyCount = 0; // Clears the notes saved in latch
        latchCleared = true; // only once
    }

    // 3. Change checking for keyboard
    if (!keypad.getKeys()) return;

    for (byte i = 0; i < LIST_MAX; i++) {
        if (!keypad.key[i].stateChanged) continue;
        
        byte index = keypad.key[i].kchar - 1; 
        if (index >= 45) continue;

        switch (keypad.key[i].kstate) {
            case PRESSED:
                if (index == KEY_SHIFT) {
                    shiftActive = true;
                    shiftPressTime = millis();
                    latchCleared = false; // Clear latch, reset sequencer
                    break;
                }

                // --- SHIFT LOGIC ---
                if (shiftActive) {

                    // LFO Wave selection
                    if (keyToLFOWave[index] != 255) {
                        VirtualControlChange(0, CC_LFOwaveSelect, keyToLFOWave[index]);
                    }
                    // Portamento/Lock Rate
                    if (keyToPORTlockRate[index] != 255) {
                        VirtualControlChange(0, CC_PORTlockRate, keyToPORTlockRate[index]);
                    }
                    
                    // Seq Rate Increment/Decrement
                    if (index == KEY_OCT_DOWN) {
                        if (SeqRateSelect > 0) SeqRateSelect--;
                    }
                    if (index == KEY_OCT_UP) {
                        if (SeqRateSelect < 3) SeqRateSelect++;
                    }
                    break;
                }

                // --- NORMAL LOGIC ---
                if (index == KEY_OCT_DOWN) {
                    if (octaveValue > 0) {
                        octaveValue--;
                        if (CurrentSeqMode == 0) VirtualControlChange(0, CC_octave, octaveValue);
                    }
                } 
                else if (index == KEY_OCT_UP) {
                    if (octaveValue < 4) {
                        octaveValue++;
                        if (CurrentSeqMode == 0) VirtualControlChange(0, CC_octave, octaveValue);
                    }
                } 
                else {
                    byte note = keyToMidiNote[index];
                    // IMPORTANT FIX: Push to sequencer ONLY if it is a valid MIDI note
                    if (note > 0 && note < 128) {
                        if (CurrentSeqMode > 0) {
                            pushKey(index);
                        } else {
                            playingNotes[index] = note + ((octaveValue - 2) * 12);
                            myNoteOn(HW_CHANNEL, (byte)playingNotes[index], HW_VELOCITY);
                        }
                    }
                }
                if (index == 12) { // Free / Trig
                        LFOmodeSelect = (LFOmodeSelect >= 2) ? 0 : (1 - LFOmodeSelect);
                    }
                    else if (index == 7) { // Rise / Fall
                        LFOmodeSelect = (LFOmodeSelect < 2) ? 2 : (LFOmodeSelect == 2 ? 3 : 2);
                    }
                break;

            case RELEASED:
                if (index == KEY_SHIFT) {
                    shiftActive = false;
                    shiftPressTime = 0;
                } 
                else if (index != KEY_OCT_DOWN && index != KEY_OCT_UP) {
                    byte note = keyToMidiNote[index];
                    // Match the press logic: only pop or stop if it was a valid note
                    if (note > 0 && note < 128) {
                        if (CurrentSeqMode == 1) {
                            popKey(index);
                        } else if (CurrentSeqMode == 0) {
                            if (playingNotes[index] != -1) {
                                myNoteOff(HW_CHANNEL, (byte)playingNotes[index], 0);
                                playingNotes[index] = -1;
                            }
                        }
                    }
                }
                break;
        }
    }
}