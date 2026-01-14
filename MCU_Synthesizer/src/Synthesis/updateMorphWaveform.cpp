// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
#include "Global_Variables\Wavetable_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
// ==================

// ----------- Update interpolated waveform -----------
void updateMorphWaveform_1() {
  const WaveFamily& wf = waveFamilies[currentWaveFamily_1];
  int numWaves = wf.numWaves;
  int morphSteps = wf.morphSteps;

  int baseIndex = currentShapeIndex_1 / morphSteps;
  int step = currentShapeIndex_1 % morphSteps;
  float t = step / (float)morphSteps;

  const int16_t* waveA = wf.waves[baseIndex];
  const int16_t* waveB = wf.waves[min(baseIndex + 1, numWaves - 1)];

  for (int i = 0; i < TABLE_SIZE; i++) {
    float fa = (float)waveA[i];
    float fb = (float)waveB[i];
    myWaveform_1[i] = (int16_t)((1.0f - t) * fa + t * fb);
  }

    waveform_1.arbitraryWaveform(myWaveform_1, TABLE_SIZE);
    waveformSub.arbitraryWaveform(myWaveform_1, TABLE_SIZE);
    if (SynthMode == 0) {
      waveformClone_1.arbitraryWaveform(myWaveform_1, TABLE_SIZE);
    }   
}

void updateMorphWaveform_2() {
  const WaveFamily& wf = waveFamilies[currentWaveFamily_2];
  int numWaves = wf.numWaves;
  int morphSteps = wf.morphSteps;

  int baseIndex = currentShapeIndex_2 / morphSteps;
  int step = currentShapeIndex_2 % morphSteps;
  float t = step / (float)morphSteps;

  const int16_t* waveA = wf.waves[baseIndex];
  const int16_t* waveB = wf.waves[min(baseIndex + 1, numWaves - 1)];

  for (int i = 0; i < TABLE_SIZE; i++) {
    float fa = (float)waveA[i];
    float fb = (float)waveB[i];
    myWaveform_2[i] = (int16_t)((1.0f - t) * fa + t * fb);
  }

    waveform_2.arbitraryWaveform(myWaveform_2, TABLE_SIZE);
    if (SynthMode == 0) {
      waveformClone_2.arbitraryWaveform(myWaveform_2, TABLE_SIZE);
    }  
}

void updateMorphWaveform_3() {
  const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
  int numWaves = wf.numWaves;
  int morphSteps = wf.morphSteps;

  int baseIndex = currentShapeIndex_3 / morphSteps;
  int step = currentShapeIndex_3 % morphSteps;
  float t = step / (float)morphSteps;

  const int16_t* waveA = wf.waves[baseIndex];
  const int16_t* waveB = wf.waves[min(baseIndex + 1, numWaves - 1)];

  for (int i = 0; i < TABLE_SIZE; i++) {
    float fa = (float)waveA[i];
    float fb = (float)waveB[i];
    myWaveform_3[i] = (int16_t)((1.0f - t) * fa + t * fb);
  }

  if (SynthMode == 0) {
    waveform_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
    waveformClone_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  } else if (SynthMode == 1) {
    waveform_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
    waveformClone_1.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
    waveformClone_2.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
    waveformClone_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  }
}