#ifndef DISPLAY_H
#define DISPLAY_H

extern bool headerDirty;
extern byte prevSynthMode;

// FUNCTION DECLARATIONS
void Display_setup();
void Display_loop();
void Display_Waveform_Snapshot();

void safeTFTpinsInit();                                  // Initialize TFT pins safely

void dmaFlush(void *ptr, int32_t bytes);                 // Flush CPU cache for DMA buffers
uint32_t computeWaveHash(const int16_t *wave);          // Compute hash of wavetable data

void drawStaticLayout();                                // Draw static UI layout
void drawRightPanelAllOnce();                           // Draw right UI panel once

void drawParams();                                      // Draw parameter values
void drawOscBlock_fast(int idx);                         // Fast redraw of oscillator block
void checkForChangesAndSetDirty();                       // Detect state changes and mark UI dirty

#endif // DISPLAY_H