#include <Arduino.h>

// === WAVETABLES ===
#include "Wavetables\Saw_Wavetables_AA.h"        // contains saw1  ... saw11
#include "Wavetables\Square_Wavetables_AA.h"     // contains square1 ... square8
#include "Wavetables\Triangle_Wavetables_AA.h"   // contains triangle1  ... triangle8
#include "Wavetables\Sine_Wavetables_AA.h"       // contains sine1 ... sineXY
#include "Wavetables\Instrument_Wavetables_AA.h" // contains instrument1  ... instrumentXY
#include "Wavetables\Voice_Wavetables_AA.h"      // contains voice1 ... voiceXY
#include "Wavetables\LFO_Wavetables.h"           // contains sine, sawRISE, sawFALL, square, noise
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables\Wavetable_Variables.h"
// ==================

#define TABLE_SIZE 256
#define WAVE_LEN 256
#define LFO_TABLE_SIZE 256
#define LFO_UPDATE_INTERVAL 5  // ms

int16_t myWaveform_1[TABLE_SIZE];  // interpolated result wave
int16_t myWaveform_2[TABLE_SIZE];
int16_t myWaveform_3[TABLE_SIZE];
int16_t myWaveform_4[TABLE_SIZE];  // LFOtype Shape - POLY mode Voice 4
int16_t myWaveform_5[TABLE_SIZE];  // LFOtype Shape - POLY mode Voice 5
int16_t myWaveform_6[TABLE_SIZE];  // LFOtype Shape - POLY mode Voice 6

// Array of pointers to individual WAVE Waveforms
extern const int16_t saw1[TABLE_SIZE];
extern const int16_t saw2[TABLE_SIZE];
extern const int16_t saw3[TABLE_SIZE];
extern const int16_t saw4[TABLE_SIZE];
extern const int16_t saw5[TABLE_SIZE];
extern const int16_t saw6[TABLE_SIZE];
extern const int16_t saw7[TABLE_SIZE];
extern const int16_t saw8[TABLE_SIZE];
extern const int16_t saw9[TABLE_SIZE];
extern const int16_t saw10[TABLE_SIZE];
extern const int16_t saw11[TABLE_SIZE];

extern const int16_t square1[TABLE_SIZE];
extern const int16_t square2[TABLE_SIZE];
extern const int16_t square3[TABLE_SIZE];
extern const int16_t square4[TABLE_SIZE];
extern const int16_t square5[TABLE_SIZE];
extern const int16_t square6[TABLE_SIZE];
extern const int16_t square7[TABLE_SIZE];
extern const int16_t square8[TABLE_SIZE];
extern const int16_t square9[TABLE_SIZE];
extern const int16_t square10[TABLE_SIZE];
extern const int16_t square11[TABLE_SIZE];
extern const int16_t square12[TABLE_SIZE];
extern const int16_t square13[TABLE_SIZE];
extern const int16_t square14[TABLE_SIZE];
extern const int16_t square15[TABLE_SIZE];
extern const int16_t square16[TABLE_SIZE];
extern const int16_t square17[TABLE_SIZE];
extern const int16_t square18[TABLE_SIZE];
extern const int16_t square19[TABLE_SIZE];
extern const int16_t square20[TABLE_SIZE];

extern const int16_t triangle1[TABLE_SIZE];
extern const int16_t triangle2[TABLE_SIZE];
extern const int16_t triangle3[TABLE_SIZE];
extern const int16_t triangle4[TABLE_SIZE];
extern const int16_t triangle5[TABLE_SIZE];
extern const int16_t triangle6[TABLE_SIZE];
extern const int16_t triangle7[TABLE_SIZE];
extern const int16_t triangle8[TABLE_SIZE];

extern const int16_t sine1[TABLE_SIZE];
extern const int16_t sine2[TABLE_SIZE];
extern const int16_t sine3[TABLE_SIZE];
extern const int16_t sine4[TABLE_SIZE];
extern const int16_t sine5[TABLE_SIZE];
extern const int16_t sine6[TABLE_SIZE];
extern const int16_t sine7[TABLE_SIZE];
extern const int16_t sine8[TABLE_SIZE];
extern const int16_t sine9[TABLE_SIZE];
extern const int16_t sine10[TABLE_SIZE];

extern const int16_t instrument1[TABLE_SIZE];
extern const int16_t instrument2[TABLE_SIZE];
extern const int16_t instrument3[TABLE_SIZE];
extern const int16_t instrument4[TABLE_SIZE];
extern const int16_t instrument5[TABLE_SIZE];
extern const int16_t instrument6[TABLE_SIZE];
extern const int16_t instrument5[TABLE_SIZE];
extern const int16_t instrument6[TABLE_SIZE];
extern const int16_t instrument7[TABLE_SIZE];
extern const int16_t instrument8[TABLE_SIZE];

extern const int16_t voice1[TABLE_SIZE];
extern const int16_t voice2[TABLE_SIZE];
extern const int16_t voice3[TABLE_SIZE];
extern const int16_t voice4[TABLE_SIZE];

// Array of pointers to LFO Waveforms
extern const int16_t sine[LFO_TABLE_SIZE];
extern const int16_t sawRISE[LFO_TABLE_SIZE];
extern const int16_t sawFALL[LFO_TABLE_SIZE];
extern const int16_t square[LFO_TABLE_SIZE];
extern const int16_t noise[LFO_TABLE_SIZE];

// Array of pointers to wavetables
const int16_t* const sawTables[] = {
  saw1, saw2, saw3, saw4, saw5, saw6, saw7, saw8, saw9, saw10, saw11
};

const int16_t* const squareTables[] = {
  square1, square2, square3, square4, square5, square6, square7, square8, square9, square10, 
  square11, square12, square13, square14, square15, square16, square17, square18, square19, square20
};

const int16_t* const triangleTables[] = {
  triangle1, triangle2, triangle3, triangle4, triangle5, triangle6, triangle7, triangle8
};

const int16_t* const sineTables[] = {
  sine1, sine2, sine3, sine4, sine5, sine6, sine7, sine8, sine9, sine10
};

const int16_t* const instrumentTables[] = {
  instrument1, instrument2, instrument3, instrument4, instrument5, instrument6, instrument7, instrument8
};

const int16_t* const voiceTables[] = {
  voice1, voice2, voice3, voice4
};

// Array of pointers for LFO wavetables
const int16_t* const LFOTables[5] = {
  sine, sawRISE, sawFALL, square, noise
};

// Definition of Wavefamilies
const WaveFamily waveFamilies[] = {
  { sawTables,       11, 10 },   // WaveFamily 0 = Saw,       11 waveforms, 10 morph steps
  { squareTables,    20,  1 },   // WaveFamily 1 = Square,    20 waveforms,  1 morph step
  { triangleTables,   8, 16 },   // WaveFamily 2 = Triangle,   8 waveforms, 16 morph steps
  { sineTables,      10,  8 },   // WaveFamily 3 = Sine,      10 waveforms,  8 morph steps
  { instrumentTables, 8,  1 },   // WaveFamily 4 = Instrument, 8 waveforms,  1 morph step
  { voiceTables,      4, 16 }    // WaveFamily 5 = Voice,      4 waveforms, 16 morph steps
};

const int NUM_WAVEFAMILIES = sizeof(waveFamilies) / sizeof(waveFamilies[0]);