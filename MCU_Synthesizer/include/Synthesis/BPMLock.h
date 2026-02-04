#ifndef BPM_LOCK_H
#define BPM_LOCK_H

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// INLINE TIMING HELPERS
inline float quarterMsFromBPM(int bpm) {
  if (bpm <= 0) return 60000.0f / 120.0f;
  return 60000.0f / (float)bpm; // ms per quarter note
}

#endif // BPM_LOCK_H