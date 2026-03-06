#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

// Display pins
#define TFT_DC       9
#define TFT_CS      10
#define TFT_RST    255  // 255 = unused, connect to 3.3V

// === Multiplexer #1 ===
#define MUX_S0          3
#define MUX_S1          4
#define MUX_S2          5
#define MUX_S3          6
#define MUX_1_control   16  // how many mux channels are being used
#define MUX_1_PIN       A15 // common analog input pin for MUX (SIG)
// ===================

// Multiplexer #1 Pins 0-15
#define MUXwave_1       0
#define MUXwave_2       1
#define MUXwave_3       2
#define MUXshape_1      3
#define MUXshape_2      4
#define MUXshape_3      5
#define MUXvolume_1     6
#define MUXvolume_2     7
#define MUXvolume_3     8
#define MUXtune_1       9
#define MUXtune_2       10
#define MUXtune_3       11
#define MUXnoise        12
#define MUXsynthMode    13
#define MUXsubOctave_1  14
#define MUXsubOctave_3  15
// ===================

// === Multiplexer #2 ===
#define MUX_2_control   16   // how many mux channels are being used
#define MUX_2_PIN       A16 // common analog input pin for MUX (SIG)
// ===================

// Multiplexer #2 Pins 0-15
#define MUX_LPF_Cutoff        0
#define MUX_LPF_Res           1
#define MUX_KBD_Track         2
#define MUX_EG_Int            3
#define MUX_HPF_Cutoff        4
#define MUX_Filter_Attack     5
#define MUX_Filter_Decay      6
#define MUX_Filter_Sustain    7
#define MUX_Filter_Release    8
#define MUX_LFO_Rate          9
#define MUX_LFO_Depth         10
#define MUX_LFO_Delay         11
#define MUX_Amplitude_Attack  12
#define MUX_Amplitude_Decay   13
#define MUX_Amplitude_Sustain 14
#define MUX_Amplitude_Release 15
// ===================

// === Multiplexer #3 ===
#define MUX_3_control   13  // how many mux channels are being used
#define MUX_3_PIN       A17 // common analog input pin for MUX (SIG)
// ===================

// Multiplexer #3 Pins 0-15
#define MUX_MasterVolume    0
#define MUXSeqGatePot       1
#define MUX_LFOtype_1       2
#define MUX_LFOtype_3       3
#define MUX_PORTswitch      4
#define MUX_Portamento_Mode 5
#define MUX_Drone_Switch    6
#define MUX_SeqMode_1       7
#define MUX_SeqMode_3       8
#define MUX_SeqOrder_1      9
#define MUX_SeqOrder_3      10
#define MUX_SeqOctaves_1    11
#define MUX_SeqOctaves_3    12
// ===================

// === LED pins ===
#define PIN_LED_Octave_1          22
#define PIN_LED_Octave_2          17
#define PIN_LED_Octave_3          16
#define PIN_LED_Octave_4          15
#define PIN_LED_Sequencer_Rate_1  14
#define PIN_LED_Sequencer_Rate_2  18
// ===================

// === IO Expander pins ===
#define PIN_LED_LFO_Mode_1        0
#define PIN_LED_LFO_Mode_2        1
#define PIN_LED_LFO_Mode_3        2
#define PIN_LED_LFO_Mode_4        3
#define PIN_LED_LFO_Wave_1        4
#define PIN_LED_LFO_Wave_2        5
#define PIN_LED_LFO_Wave_3        6
#define PIN_LED_LFO_Wave_4        7
#define PIN_LED_Sequencer_Rate_3  8
#define PIN_LED_Sequencer_Rate_4  9
#define PIN_LED_LFO_Wave_5        10
#define PIN_LED_Portamento_Rate_1 11
#define PIN_LED_Portamento_Rate_2 12
#define PIN_LED_Portamento_Rate_3 13
#define PIN_LED_Portamento_Rate_4 14
#define PIN_LED_Portamento_Rate_5 15
// ===================

// === Encoder KY-040 pins ===
#define Encoder_SW   0   // push button
#define Encoder_DT   1
#define Encoder_CLK  2
// ===================

// === Hardware Keyboard pins ===
// Matrix - organised in rows and columns
#define Row_1     30
#define Row_2     31  
#define Row_3     32  
#define Column_1  27
#define Column_2  28
#define Column_3  29
// ===================

#endif // PINS_CONFIG_H