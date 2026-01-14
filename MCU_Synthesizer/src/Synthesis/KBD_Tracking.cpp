// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\KBD_Tracking.h"
// ==================

// PRECOMPUTE KEYBOARD MULTIPLIERS
void recomputeKBDmult() {
  for (int n = 0; n < 128; ++n) {
    float diff = (float)(n - KBD_BASE_NOTE);
    KBDmult[n] = powf(2.0f, diff / 12.0f); // multiplicator for full 1:1 tracking
  }
}

// Re-calculates KBDtrackingMod for all voices according to current note + octave
void updateKBDtrackingMods() {
  // if LPFcutoffFreq is 0 (or very small), avoid dividing by zero
  float baseLPF = (LPFcutoffFreq > 0.0001f) ? LPFcutoffFreq : 1.0f;

  // UNISON or POLY: calculate correct index / fallback
  if (SynthMode == 0) {
    KBDtrackingMod_1 = voiceActive_1 ? computeVoiceLPFCutoff(voiceNote_1 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_2 = voiceActive_2 ? computeVoiceLPFCutoff(voiceNote_2 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_3 = voiceActive_3 ? computeVoiceLPFCutoff(voiceNote_3 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_4 = voiceActive_4 ? computeVoiceLPFCutoff(voiceNote_4 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_5 = voiceActive_5 ? computeVoiceLPFCutoff(voiceNote_5 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_6 = voiceActive_6 ? computeVoiceLPFCutoff(voiceNote_6 + octave) / baseLPF : 1.0f;
    if (unisonTriad == 0) {
      KBDtrackingMod_7 = computeVoiceLPFCutoff(voiceNote_1 + octave + SubOctave) / baseLPF;
    } else {
      KBDtrackingMod_7 = computeVoiceLPFCutoff(voiceNote_4 + octave + SubOctave) / baseLPF;
    }
  } else {
    // for every voice if active, else set to 1.0
    KBDtrackingMod_1 = voiceActive_1 ? computeVoiceLPFCutoff(voiceNote_1 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_2 = voiceActive_2 ? computeVoiceLPFCutoff(voiceNote_2 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_3 = voiceActive_3 ? computeVoiceLPFCutoff(voiceNote_3 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_4 = voiceActive_4 ? computeVoiceLPFCutoff(voiceNote_4 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_5 = voiceActive_5 ? computeVoiceLPFCutoff(voiceNote_5 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_6 = voiceActive_6 ? computeVoiceLPFCutoff(voiceNote_6 + octave) / baseLPF : 1.0f;
    KBDtrackingMod_7 = voiceActive_SUB ? computeVoiceLPFCutoff(voiceNote_1 + octave + SubOctave) / baseLPF : 1.0f;
  }

  // small sanity clamp (avoid extremes)
  if (!isfinite(KBDtrackingMod_1) || KBDtrackingMod_1 <= 0.0001f) KBDtrackingMod_1 = 0.0001f;
  if (!isfinite(KBDtrackingMod_2) || KBDtrackingMod_2 <= 0.0001f) KBDtrackingMod_2 = 0.0001f;
  if (!isfinite(KBDtrackingMod_3) || KBDtrackingMod_3 <= 0.0001f) KBDtrackingMod_3 = 0.0001f;
  if (!isfinite(KBDtrackingMod_4) || KBDtrackingMod_4 <= 0.0001f) KBDtrackingMod_4 = 0.0001f;
  if (!isfinite(KBDtrackingMod_5) || KBDtrackingMod_5 <= 0.0001f) KBDtrackingMod_5 = 0.0001f;
  if (!isfinite(KBDtrackingMod_6) || KBDtrackingMod_6 <= 0.0001f) KBDtrackingMod_6 = 0.0001f;
  if (!isfinite(KBDtrackingMod_7) || KBDtrackingMod_7 <= 0.0001f) KBDtrackingMod_7 = 0.0001f;

  KBDtrackingMod_1 = constrain(KBDtrackingMod_1, 0.0001f, 1.0f);
  KBDtrackingMod_2 = constrain(KBDtrackingMod_2, 0.0001f, 1.0f);
  KBDtrackingMod_3 = constrain(KBDtrackingMod_3, 0.0001f, 1.0f);
  KBDtrackingMod_4 = constrain(KBDtrackingMod_4, 0.0001f, 1.0f);
  KBDtrackingMod_5 = constrain(KBDtrackingMod_5, 0.0001f, 1.0f);
  KBDtrackingMod_6 = constrain(KBDtrackingMod_6, 0.0001f, 1.0f);
  KBDtrackingMod_7 = constrain(KBDtrackingMod_7, 0.0001f, 1.0f);
}