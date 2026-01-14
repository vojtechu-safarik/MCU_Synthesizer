#ifndef WAVETABLE_VARIABLES_H
#define WAVETABLE_VARIABLES_H

#include <Arduino.h>

// ===== Constants =====
#define TABLE_SIZE 256
#define WAVE_LEN 256
#define LFO_TABLE_SIZE 256
#define LFO_UPDATE_INTERVAL 5  // ms

// ===== Runtime wave buffers =====
extern int16_t myWaveform_1[TABLE_SIZE];
extern int16_t myWaveform_2[TABLE_SIZE];
extern int16_t myWaveform_3[TABLE_SIZE];
extern int16_t myWaveform_4[TABLE_SIZE];
extern int16_t myWaveform_5[TABLE_SIZE];
extern int16_t myWaveform_6[TABLE_SIZE];

// ===== Structure describing WaveFamily =====
struct WaveFamily {
  const int16_t* const* waves;  // array of pointers to individual waveforms
  int numWaves;                 // number of waveforms in a family
  int morphSteps;               // number of morphing steps between two waveforms
};

// ===== Individual oscillator wavetables =====
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
extern const int16_t instrument7[TABLE_SIZE];
extern const int16_t instrument8[TABLE_SIZE];

extern const int16_t voice1[TABLE_SIZE];
extern const int16_t voice2[TABLE_SIZE];
extern const int16_t voice3[TABLE_SIZE];
extern const int16_t voice4[TABLE_SIZE];

// ===== LFO wavetables =====
extern const int16_t sine[LFO_TABLE_SIZE];
extern const int16_t sawRISE[LFO_TABLE_SIZE];
extern const int16_t sawFALL[LFO_TABLE_SIZE];
extern const int16_t square[LFO_TABLE_SIZE];
extern const int16_t noise[LFO_TABLE_SIZE];

// ===== Wavetable pointer arrays =====
extern const int16_t* const sawTables[];
extern const int16_t* const squareTables[];
extern const int16_t* const triangleTables[];
extern const int16_t* const sineTables[];
extern const int16_t* const instrumentTables[];
extern const int16_t* const voiceTables[];

extern const int16_t* const LFOTables[5];

// ===== WaveFamilies =====
extern const WaveFamily waveFamilies[];
extern const int NUM_WAVEFAMILIES;

#endif // WAVETABLE_VARIABLES_H