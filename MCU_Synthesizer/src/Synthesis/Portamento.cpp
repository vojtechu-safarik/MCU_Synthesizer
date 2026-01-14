// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Portamento.h"
// ==================

void PortamentoUpdate() {
  unsigned long now = micros();
  float dt_ms = 0.0f;
  if (lastPortamentoMicros != 0) {
    dt_ms = (now - lastPortamentoMicros) / 1000.0f; // ms since last call
  }
  lastPortamentoMicros = now;

  // if portamento is off, set multiplicators to one and end
  if (PORTswitch == 0) {
    PORTdetune_1 = PORTdetune_2 = PORTdetune_3 =
    PORTdetune_4 = PORTdetune_5 = PORTdetune_6 = 1.0f;
    // dont affect PORTactive_*, that is being done by noteOn/Off
    return;
  }

  switch (PORTmodeSelect) {
    case 0: { // Fixed Time (time controlled by subdivision according to BPM)
      float subdivMs = PortamentoSubdivisionMsFromLockIndex((int)PORTlockRate, GlobalBPM);
      // glideTime = subdivision * controlFraction (0 .. 1)
      float glideTimeMs = subdivMs * portTimeControl;
      if (glideTimeMs < 1.0f) glideTimeMs = 1.0f; // safety limit
      // increment is relative (fraction of glide per this step)
      float fracStep = (glideTimeMs > 0.0f && dt_ms > 0.0f) ? (dt_ms / glideTimeMs) : 1.0f;

      // VOICE 1
      if (PORTactive_1) {
        PORTonce_1 += fracStep;
        if (PORTonce_1 >= 1.0f) { PORTonce_1 = 1.0f; PORTactive_1 = false; PORTstop_1 = true; }
        PORTcurrentSemitone_1 = PORTstartSemitone_1 + PORTonce_1 * (PORTtargetSemitone_1 - PORTstartSemitone_1);
      } else {
        // hold target (enables instaneous setting if not active)
        PORTcurrentSemitone_1 = PORTtargetSemitone_1;
        PORTonce_1 = 1.0f;
      }

      // VOICE 2
      if (PORTactive_2) {
        PORTonce_2 += fracStep;
        if (PORTonce_2 >= 1.0f) { PORTonce_2 = 1.0f; PORTactive_2 = false; PORTstop_2 = true; }
        PORTcurrentSemitone_2 = PORTstartSemitone_2 + PORTonce_2 * (PORTtargetSemitone_2 - PORTstartSemitone_2);
      } else {
        PORTcurrentSemitone_2 = PORTtargetSemitone_2; PORTonce_2 = 1.0f;
      }

      // VOICE 3
      if (PORTactive_3) {
        PORTonce_3 += fracStep;
        if (PORTonce_3 >= 1.0f) { PORTonce_3 = 1.0f; PORTactive_3 = false; PORTstop_3 = true; }
        PORTcurrentSemitone_3 = PORTstartSemitone_3 + PORTonce_3 * (PORTtargetSemitone_3 - PORTstartSemitone_3);
      } else {
        PORTcurrentSemitone_3 = PORTtargetSemitone_3; PORTonce_3 = 1.0f;
      }

      // VOICE 4
      if (PORTactive_4) {
        PORTonce_4 += fracStep;
        if (PORTonce_4 >= 1.0f) { PORTonce_4 = 1.0f; PORTactive_4 = false; PORTstop_4 = true; }
        PORTcurrentSemitone_4 = PORTstartSemitone_4 + PORTonce_4 * (PORTtargetSemitone_4 - PORTstartSemitone_4);
      } else {
        PORTcurrentSemitone_4 = PORTtargetSemitone_4; PORTonce_4 = 1.0f;
      }

      // VOICE 5
      if (PORTactive_5) {
        PORTonce_5 += fracStep;
        if (PORTonce_5 >= 1.0f) { PORTonce_5 = 1.0f; PORTactive_5 = false; PORTstop_5 = true; }
        PORTcurrentSemitone_5 = PORTstartSemitone_5 + PORTonce_5 * (PORTtargetSemitone_5 - PORTstartSemitone_5);
      } else {
        PORTcurrentSemitone_5 = PORTtargetSemitone_5; PORTonce_5 = 1.0f;
      }

      // VOICE 6
      if (PORTactive_6) {
        PORTonce_6 += fracStep;
        if (PORTonce_6 >= 1.0f) { PORTonce_6 = 1.0f; PORTactive_6 = false; PORTstop_6 = true; }
        PORTcurrentSemitone_6 = PORTstartSemitone_6 + PORTonce_6 * (PORTtargetSemitone_6 - PORTstartSemitone_6);
      } else {
        PORTcurrentSemitone_6 = PORTtargetSemitone_6; PORTonce_6 = 1.0f;
      }

      break;
    }

    case 1: { // Fixed Rate (tempo-locked speed controlled by subdivision)
              // if rateSemitones == 12.0 and subdivMs equals a quarter note, we travel an octave during a quarter note
      float subdivMs = PortamentoSubdivisionMsFromLockIndex((int)PORTlockRate, GlobalBPM);
      if (subdivMs <= 0.0f) subdivMs = 1.0f; // safety

      float semitonePerMs = 0.0f;
      if (portRateSemitonesPerSubdivision > 0.0f) {
        semitonePerMs = portRateSemitonesPerSubdivision / subdivMs;
      }

      if (dt_ms <= 0.0f) dt_ms = 0.0f;

      // VOICE 1
      {
        float remaining = PORTtargetSemitone_1 - PORTcurrentSemitone_1;
        if (PORTactive_1 && fabsf(remaining) > 0.0001f && semitonePerMs > 0.0f) {
          float step = (remaining > 0.0f ? 1.0f : -1.0f) * semitonePerMs * dt_ms;
          if (fabsf(step) >= fabsf(remaining)) {
            PORTcurrentSemitone_1 = PORTtargetSemitone_1;
            PORTactive_1 = false; PORTstop_1 = true;
          } else {
            PORTcurrentSemitone_1 += step;
            float totalDelta = PORTtargetSemitone_1 - PORTstartSemitone_1;
            if (fabsf(totalDelta) > 0.000001f) {
              PORTonce_1 = (PORTcurrentSemitone_1 - PORTstartSemitone_1) / totalDelta;
              if (PORTonce_1 < 0.0f) PORTonce_1 = 0.0f;
              if (PORTonce_1 > 1.0f) PORTonce_1 = 1.0f;
            } else {
              PORTonce_1 = 1.0f;
            }
          }
        } else {
          PORTcurrentSemitone_1 = PORTtargetSemitone_1;
          PORTonce_1 = 1.0f;
        }
      }

      // VOICE 2
      {
        float remaining = PORTtargetSemitone_2 - PORTcurrentSemitone_2;
        if (PORTactive_2 && fabsf(remaining) > 0.0001f && semitonePerMs > 0.0f) {
          float step = (remaining > 0.0f ? 1.0f : -1.0f) * semitonePerMs * dt_ms;
          if (fabsf(step) >= fabsf(remaining)) { 
            PORTcurrentSemitone_2 = PORTtargetSemitone_2; 
            PORTactive_2 = false; 
            PORTstop_2 = true; 
            PORTonce_2 = 1.0f; 
          } else { 
            PORTcurrentSemitone_2 += step; 
            float totalDelta = PORTtargetSemitone_2 - PORTstartSemitone_2; 
            if (fabsf(totalDelta) > 0.000001f) { 
              PORTonce_2 = (PORTcurrentSemitone_2 - PORTstartSemitone_2) / totalDelta; 
              if (PORTonce_2 < 0.0f) PORTonce_2 = 0.0f; 
              if (PORTonce_2 > 1.0f) PORTonce_2 = 1.0f; 
            } else {
              PORTonce_2 = 1.0f;
            }
          }
        } else { 
          PORTcurrentSemitone_2 = PORTtargetSemitone_2; 
          PORTonce_2 = 1.0f; 
        }
      }

      // VOICE 3
      {
        float remaining = PORTtargetSemitone_3 - PORTcurrentSemitone_3;
        if (PORTactive_3 && fabsf(remaining) > 0.0001f && semitonePerMs > 0.0f) {
          float step = (remaining > 0.0f ? 1.0f : -1.0f) * semitonePerMs * dt_ms;
          if (fabsf(step) >= fabsf(remaining)) { 
            PORTcurrentSemitone_3 = PORTtargetSemitone_3; 
            PORTactive_3 = false; 
            PORTstop_3 = true; 
            PORTonce_3 = 1.0f; 
          } else { 
            PORTcurrentSemitone_3 += step; 
            float totalDelta = PORTtargetSemitone_3 - PORTstartSemitone_3; 
            if (fabsf(totalDelta) > 0.000001f) { 
              PORTonce_3 = (PORTcurrentSemitone_3 - PORTstartSemitone_3) / totalDelta; 
              if (PORTonce_3 < 0.0f) PORTonce_3 = 0.0f; 
              if (PORTonce_3 > 1.0f) PORTonce_3 = 1.0f; 
            } else {
              PORTonce_3 = 1.0f;
            }
          }
        } else { 
          PORTcurrentSemitone_3 = PORTtargetSemitone_3; 
          PORTonce_3 = 1.0f; 
        }
      }

      // VOICE 4
      {
        float remaining = PORTtargetSemitone_4 - PORTcurrentSemitone_4;
        if (PORTactive_4 && fabsf(remaining) > 0.0001f && semitonePerMs > 0.0f) {
          float step = (remaining > 0.0f ? 1.0f : -1.0f) * semitonePerMs * dt_ms;
          if (fabsf(step) >= fabsf(remaining)) { 
            PORTcurrentSemitone_4 = PORTtargetSemitone_4; 
            PORTactive_4 = false; 
            PORTstop_4 = true; 
            PORTonce_4 = 1.0f; 
          } else { 
            PORTcurrentSemitone_4 += step; 
            float totalDelta = PORTtargetSemitone_4 - PORTstartSemitone_4; 
            if (fabsf(totalDelta) > 0.000001f) { 
              PORTonce_4 = (PORTcurrentSemitone_4 - PORTstartSemitone_4) / totalDelta; 
              if (PORTonce_4 < 0.0f) PORTonce_4 = 0.0f; 
              if (PORTonce_4 > 1.0f) PORTonce_4 = 1.0f; 
            } else {
              PORTonce_4 = 1.0f;
            }
          }
        } else { 
          PORTcurrentSemitone_4 = PORTtargetSemitone_4; 
          PORTonce_4 = 1.0f; 
        }
      }

      // VOICE 5
      {
        float remaining = PORTtargetSemitone_5 - PORTcurrentSemitone_5;
        if (PORTactive_5 && fabsf(remaining) > 0.0001f && semitonePerMs > 0.0f) {
          float step = (remaining > 0.0f ? 1.0f : -1.0f) * semitonePerMs * dt_ms;
          if (fabsf(step) >= fabsf(remaining)) { 
            PORTcurrentSemitone_5 = PORTtargetSemitone_5; 
            PORTactive_5 = false; 
            PORTstop_5 = true; 
            PORTonce_5 = 1.0f; 
          } else { 
            PORTcurrentSemitone_5 += step; 
            float totalDelta = PORTtargetSemitone_5 - PORTstartSemitone_5; 
            if (fabsf(totalDelta) > 0.000001f) { 
              PORTonce_5 = (PORTcurrentSemitone_5 - PORTstartSemitone_5) / totalDelta; 
              if (PORTonce_5 < 0.0f) PORTonce_5 = 0.0f; 
              if (PORTonce_5 > 1.0f) PORTonce_5 = 1.0f; 
            } else {
              PORTonce_5 = 1.0f;
            }
          }
        } else { 
          PORTcurrentSemitone_5 = PORTtargetSemitone_5; 
          PORTonce_5 = 1.0f; 
        }
      }

      // VOICE 6
      {
        float remaining = PORTtargetSemitone_6 - PORTcurrentSemitone_6;
        if (PORTactive_6 && fabsf(remaining) > 0.0001f && semitonePerMs > 0.0f) {
          float step = (remaining > 0.0f ? 1.0f : -1.0f) * semitonePerMs * dt_ms;
          if (fabsf(step) >= fabsf(remaining)) { 
            PORTcurrentSemitone_6 = PORTtargetSemitone_6; 
            PORTactive_6 = false; 
            PORTstop_6 = true; 
            PORTonce_6 = 1.0f; 
          } else { 
            PORTcurrentSemitone_6 += step; 
            float totalDelta = PORTtargetSemitone_6 - PORTstartSemitone_6; 
            if (fabsf(totalDelta) > 0.000001f) { 
              PORTonce_6 = (PORTcurrentSemitone_6 - PORTstartSemitone_6) / totalDelta; 
              if (PORTonce_6 < 0.0f) PORTonce_6 = 0.0f; 
              if (PORTonce_6 > 1.0f) PORTonce_6 = 1.0f; 
            } else {
              PORTonce_6 = 1.0f;
            }
          }
        } else { 
          PORTcurrentSemitone_6 = PORTtargetSemitone_6; 
          PORTonce_6 = 1.0f; 
        }
      }

      break;
    }

    default:
      // unknown mode --> disable portamento (safely)
      PORTdetune_1 = PORTdetune_2 = PORTdetune_3 =
      PORTdetune_4 = PORTdetune_5 = PORTdetune_6 = 1.0f;
      return;
  }

  // Conversion to frequency multipliers (applied in updateVoices together with Tune)
  PORTdetune_1 = powf(2.0f, PORTcurrentSemitone_1 / 12.0f);
  PORTdetune_2 = powf(2.0f, PORTcurrentSemitone_2 / 12.0f);
  PORTdetune_3 = powf(2.0f, PORTcurrentSemitone_3 / 12.0f);
  PORTdetune_4 = powf(2.0f, PORTcurrentSemitone_4 / 12.0f);
  PORTdetune_5 = powf(2.0f, PORTcurrentSemitone_5 / 12.0f);
  PORTdetune_6 = powf(2.0f, PORTcurrentSemitone_6 / 12.0f);
}