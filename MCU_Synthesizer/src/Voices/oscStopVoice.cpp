// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\oscStopVoice.h"
// ==================

void oscStopVoice_1() {
  envelope_Amplitude_1.noteOff();
  // free the voice for possible fallback, but signalise, that release is still on
  voiceActive_1 = false;      // voice is free for allocation of the next note (fallback)
  voiceReleasing_1 = true;    // but the LFO/oneshot is still running and affecting the envelope release
}

void oscStopVoice_2() {
  envelope_Amplitude_2.noteOff();
  voiceActive_2 = false;
  voiceReleasing_2 = true;
}

void oscStopVoice_3() {
  envelope_Amplitude_3.noteOff();
  voiceActive_3 = false;
  voiceReleasing_3 = true;
}

void oscStopVoice_4() {
  envelopeClone_Amplitude_1.noteOff();
  voiceActive_4 = false;
  voiceReleasing_4 = true;
}

void oscStopVoice_5() {
  envelopeClone_Amplitude_2.noteOff();
  voiceActive_5 = false;
  voiceReleasing_5 = true;
}

void oscStopVoice_6() {
  envelopeClone_Amplitude_3.noteOff();
  voiceActive_6 = false;
  voiceReleasing_6 = true;
}