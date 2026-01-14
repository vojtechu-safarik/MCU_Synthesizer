// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\updateMorphWaveform.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\resetAllVoices.h"
// ==================

// resets the voices upon SynthMode change 
void resetAllVoices() {
  voiceActive_1 = false; voiceNote_1 = 0; voiceReleasing_1 = false; envelope_Amplitude_1.noteOff();
  voiceActive_2 = false; voiceNote_2 = 0; voiceReleasing_2 = false; envelope_Amplitude_2.noteOff();
  voiceActive_3 = false; voiceNote_3 = 0; voiceReleasing_3 = false; envelope_Amplitude_3.noteOff();
  mixer_1.gain(0, 0.5); mixer_1.gain(1, 0.5); mixer_1.gain(2, 0.5);
  voiceActive_4 = false; voiceNote_4 = 0; voiceReleasing_4 = false; envelopeClone_Amplitude_1.noteOff();
  voiceActive_5 = false; voiceNote_5 = 0; voiceReleasing_5 = false; envelopeClone_Amplitude_2.noteOff();
  voiceActive_6 = false; voiceNote_6 = 0; voiceReleasing_6 = false; envelopeClone_Amplitude_3.noteOff();
  voiceActive_SUB = false;                envelopeSub_Amplitude.noteOff();
  mixer_2.gain(0, 0.5); mixer_2.gain(1, 0.5); mixer_2.gain(2, 0.5);

  updateMorphWaveform_1();
  updateMorphWaveform_2();
  updateMorphWaveform_3();
}