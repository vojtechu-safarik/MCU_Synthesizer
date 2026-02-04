#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

// Display pins
#define TFT_DC       9
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V

// === MULTIPLEXER #1 ===
#define MUX_S0          3
#define MUX_S1          4
#define MUX_S2          5
#define MUX_S3          6
#define MUX_1_control   7   // how many mux channels are being used
#define MUX_1_PIN       A15 // common analog input pin for MUX (SIG)
// ===================

// MULTIPLEXER #1 Pins 0-15
#define MUXshape_1      0
#define MUXshape_2      1
#define MUXshape_3      2
#define MUXSeqGatePot   3
#define MUX_SynthMode   4
#define MUX_LFOtype_1   5
#define MUX_LFOtype_3   6
// ===================

// === MULTIPLEXER #2 ===
#define MUX_2_control   4   // how many mux channels are being used
#define MUX_2_PIN       A16 // common analog input pin for MUX (SIG)
// ===================

// MULTIPLEXER #2 Pins 0-15
#define MUX_LPF_Cutoff  0
#define MUX_LPF_Res     1
#define MUX_KBD_Track   2
#define MUX_EG_Int      3
// ===================

// Switches pin numbers
#define SWSeqMode_1 6
#define SWSeqMode_3 7

#define SWSeqOrder_1 8
#define SWSeqOrder_3 18

#define SWSeqOctave_1 19
#define SWSeqOctave_3 20
// ===================

// Encoder KY-040 pin numbers
#define ENC_SW   0   // push button
#define ENC_DT   1
#define ENC_CLK  2

#endif // PINS_CONFIG_H