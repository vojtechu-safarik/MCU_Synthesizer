// ==========================================
// ====== Headers required by function ======

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\LFO.h"
#include "Synthesis\KBD_Tracking.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\oscPlayVoice.h"
#include "Voices\oscStopVoice.h"
// ==================

// === CONTROLS ===
#include "Controls\MIDI_Control.h"
// ==================

// ----------- Keyboard -----------
void myNoteOff(byte channel, byte note, byte velocity) {
  // if (note < 23 || note > 107) return; // not used
  if (SynthMode == 0) {
    if (unisonTriad == 0) {
      if (voiceActive_1 && voiceNote_1 == note) {
          oscStopVoice_1();
          envelopeSub_Amplitude.noteOff(); voiceActive_SUB = false; }
      if (voiceActive_2 && voiceNote_2 == note) oscStopVoice_2();
      if (voiceActive_3 && voiceNote_3 == note) oscStopVoice_3();
      envelopeNoise_1.noteOff();
    } else if (unisonTriad == 1) {
      if (voiceActive_4 && voiceNote_4 == note) {
          oscStopVoice_4();
          envelopeSub_Amplitude.noteOff(); voiceActive_SUB = false; }
      if (voiceActive_5 && voiceNote_5 == note) oscStopVoice_5();
      if (voiceActive_6 && voiceNote_6 == note) oscStopVoice_6();
      envelopeNoise_1.noteOff();
    }
  } 
  if (SynthMode == 1) {
    if (voiceActive_1 && voiceNote_1 == note) {
        oscStopVoice_1();
        envelopeSub_Amplitude.noteOff(); voiceActive_SUB = false; }
    if (voiceActive_2 && voiceNote_2 == note) oscStopVoice_2();
    if (voiceActive_3 && voiceNote_3 == note) oscStopVoice_3();
    if (voiceActive_4 && voiceNote_4 == note) oscStopVoice_4();
    if (voiceActive_5 && voiceNote_5 == note) oscStopVoice_5();
    if (voiceActive_6 && voiceNote_6 == note) oscStopVoice_6();

  if (!voiceActive_1 && !voiceActive_2 && !voiceActive_3 && !voiceActive_4 && !voiceActive_5 && !voiceActive_6) {
    envelopeNoise_1.noteOff();
  }

  }
}