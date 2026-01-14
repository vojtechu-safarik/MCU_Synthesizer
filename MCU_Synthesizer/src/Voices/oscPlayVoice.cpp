#include <Arduino.h>

// ==========================================
// ====== Headers required by function ======

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\KBD_Tracking.h"
// ==================

void oscPlayVoice_1(byte note) {
  float freq = noteFreqs[note + octave];
  // waveform_1.amplitude(globalVelocity * DIV127);
  envelope_Amplitude_1.noteOn();
  envelopeNoise_1.noteOn();
  voiceNote_1 = note;
  voiceFreq_1 = freq;
  voiceActive_1 = true;
  voiceReleasing_1 = false;

  if ((SynthMode == 1) || ((SynthMode == 0) && (unisonTriad == 0))) {
    // waveformSub.amplitude(globalVelocity * DIV127);
    envelopeSub_Amplitude.noteOn();
    voiceActive_SUB = true;
  }

  updateKBDtrackingMods();
}

void oscPlayVoice_2(byte note) {
  float freq = noteFreqs[note + octave];
  // waveform_2.amplitude(globalVelocity * DIV127);
  envelope_Amplitude_2.noteOn();
  envelopeNoise_1.noteOn();
  voiceNote_2 = note;
  voiceFreq_2 = freq;
  voiceActive_2 = true;
  voiceReleasing_2 = false;

  updateKBDtrackingMods();
}

void oscPlayVoice_3(byte note) {
  float freq = noteFreqs[note + octave];
  // waveform_3.amplitude(globalVelocity * DIV127);
  envelope_Amplitude_3.noteOn();
  envelopeNoise_1.noteOn();
  voiceNote_3 = note;
  voiceFreq_3 = freq;
  voiceActive_3 = true;
  voiceReleasing_3 = false;

  updateKBDtrackingMods();
}

void oscPlayVoice_4(byte note) {
  float freq = noteFreqs[note + octave];
  // waveformClone_1.amplitude(globalVelocity * DIV127);
  envelopeClone_Amplitude_1.noteOn();
  envelopeNoise_1.noteOn();
  voiceNote_4 = note;
  voiceFreq_4 = freq;
  voiceActive_4 = true;
  voiceReleasing_4 = false;

  if ((SynthMode == 0) && (unisonTriad == 1)) {
    // waveformSub.amplitude(globalVelocity * DIV127);
    envelopeSub_Amplitude.noteOn();
    voiceActive_SUB = true;
  }

  updateKBDtrackingMods();
}

void oscPlayVoice_5(byte note) {
  float freq = noteFreqs[note + octave];
  // waveformClone_2.amplitude(globalVelocity * DIV127);
  envelopeClone_Amplitude_2.noteOn();
  envelopeNoise_1.noteOn();
  voiceNote_5 = note;
  voiceFreq_5 = freq;
  voiceActive_5 = true;
  voiceReleasing_5 = false;

  updateKBDtrackingMods();
}

void oscPlayVoice_6(byte note) {
  float freq = noteFreqs[note + octave];
  // waveformClone_3.amplitude(globalVelocity * DIV127);
  envelopeClone_Amplitude_3.noteOn();
  envelopeNoise_1.noteOn();
  voiceNote_6 = note;
  voiceFreq_6 = freq;
  voiceActive_6 = true;
  voiceReleasing_6 = false;

  updateKBDtrackingMods();
}