#ifndef KBD_TRACKING_H
#define KBD_TRACKING_H

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// INLINE HELPERS
inline float computeVoiceLPFCutoff(int noteIndex) {
  // safety: index 0 ... 127
  int idx = noteIndex;
  if (idx < 0) idx = 0;
  if (idx > 127) idx = 127;

  float mult = KBDmult[idx]; // pre-calculated 2^((n-base)/12)
  // linear interpolation between 1.0 and mult per KBDtracking
  float factor = 1.0f + (mult - 1.0f) * KBDtracking;
  float cutoff = LPFcutoffFreq * factor;

  // Clamp for hearable range
  if (cutoff < 20.0f) cutoff = 20.0f;
  if (cutoff > 20000.0f) cutoff = 20000.0f;
  
  return cutoff;
}

// API
void recomputeKBDmult();                                // Recompute keyboard tracking multiplier
float computeVoiceLPFCutoff(int noteIndex);             // Compute per-voice LPF cutoff frequency
void updateKBDtrackingMods();                           // Update keyboard tracking modulation

#endif // KBD_TRACKING_H