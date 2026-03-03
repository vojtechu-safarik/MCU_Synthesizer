#ifndef PORTAMENTO_H
#define PORTAMENTO_H

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"  // možná smazat, zbytečný
// ==================

// Inline timing helper
inline float quarterMsFromBPM(int bpm) {
  if (bpm <= 0) return 60000.0f / 120.0f;
  return 60000.0f / (float)bpm; // ms per quarter note
}

inline float PortamentoSubdivisionMsFromLockIndex(int idx, int bpm) {
  if (idx < 0) idx = 0;
  if (idx > 4) idx = 4;
  float q = quarterMsFromBPM(bpm);
  return q * Portamento_Lock_Factors_by_Index[idx];
}

// FUNCTIONS
float PortamentoSubdivisionMsFromLockIndex(int idx, int bpm);       // Portamento subdivision timing
void PortamentoUpdate();                                            // Update portamento state

#endif // PORTAMENTO_H