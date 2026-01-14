#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

// Display pins
#define TFT_DC       9
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V

// === MULTIPLEXER #1 ===
#define MUX_S0      14
#define MUX_S1      15
#define MUX_S2      16
#define MUX_S3      17
#define MUX_control 3   // how many mux channels are being used
#define MUX_PIN     A16 // common analog input pin for MUX (SIG)
// ===================

// MULTIPLEXER #1 Pins 0-15
#define MUXshape_1  0
#define MUXshape_2  1
#define MUXshape_3  2
// ===================

// Switches pin numbers
#define SWsynthMode 3
#define SWLFOtype_1 4
#define SWLFOtype_3 5
// ===================

// Encoder KY-040 pin numbers
#define ENC_SW   0   // push button
#define ENC_DT   1
#define ENC_CLK  2

#endif // PINS_CONFIG_H