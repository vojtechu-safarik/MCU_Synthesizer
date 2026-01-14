#ifndef RESET_ALL_VOICES_H
#define RESET_ALL_VOICES_H


#include <Arduino.h>

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
// ==================

// =========================================================
// VOICE STATE (defined elsewhere, zde jen extern)
// =========================================================
extern bool voiceActive_1;
extern bool voiceActive_2;
extern bool voiceActive_3;
extern bool voiceActive_4;
extern bool voiceActive_5;
extern bool voiceActive_6;
extern bool voiceActive_SUB;

extern byte voiceNote_1;
extern byte voiceNote_2;
extern byte voiceNote_3;
extern byte voiceNote_4;
extern byte voiceNote_5;
extern byte voiceNote_6;

extern bool voiceReleasing_1;
extern bool voiceReleasing_2;
extern bool voiceReleasing_3;
extern bool voiceReleasing_4;
extern bool voiceReleasing_5;
extern bool voiceReleasing_6;

// API
void resetAllVoices();                                          // Reset all voice states

#endif // RESET_ALL_VOICES_H