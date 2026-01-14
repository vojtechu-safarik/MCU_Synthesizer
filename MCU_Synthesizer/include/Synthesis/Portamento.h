#ifndef PORTAMENTO_H
#define PORTAMENTO_H

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\BPMLock.h"
// ==================

inline float PortamentoSubdivisionMsFromLockIndex(int idx, int bpm) {
  if (idx < 0) idx = 0;
  if (idx > 6) idx = 6;
  float q = quarterMsFromBPM(bpm);
  return q * PORTAMENTO_LOCK_FACTORS_BY_INDEX[idx];
}

// FUNCTIONS
float PortamentoSubdivisionMsFromLockIndex(int idx, int bpm); // Portamento subdivision timing
void PortamentoUpdate();                                            // Update portamento state

#endif // PORTAMENTO_H