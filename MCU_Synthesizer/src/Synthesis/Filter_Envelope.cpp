// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\Filter_Envelope.h"
// ==================

unsigned long envelopeStartTime[FILTER_ENV_VOICES];
unsigned long releaseStartTime[FILTER_ENV_VOICES];

bool prevActiveArr[FILTER_ENV_VOICES]; // previous active state per voice
bool prevNoteOnArr[FILTER_ENV_VOICES];    // track previous voiceActive state (note-on edge)
bool prevReleasingArr[FILTER_ENV_VOICES]; // track previous voiceReleasing state (release edge)

float filterEnvelope_Attack_Coeff[FILTER_ENV_VOICES];
float filterEnvelope_Decay_Coeff[FILTER_ENV_VOICES];
float filterEnvelope_Sustain_Coeff[FILTER_ENV_VOICES];
float filterEnvelope_Release_Coeff[FILTER_ENV_VOICES];
float filterEnvelope_Coefficient[FILTER_ENV_VOICES]; // Result coefficient for every voice

// Release continues dynamically during note-off
float savedAttackCoef[FILTER_ENV_VOICES];
float savedDecayCoef[FILTER_ENV_VOICES];
float savedSustainCoef[FILTER_ENV_VOICES];

// FUNCTION IMPLEMENTATIONS
void initEnvelopesPerVoice() {
  for (int i = 0; i < FILTER_ENV_VOICES; ++i) {
    envelopeStartTime[i] = 0;
    releaseStartTime[i] = 0;
    prevActiveArr[i] = false;
    prevNoteOnArr[i] = false;        // nově
    prevReleasingArr[i] = false;     // nově
    filterEnvelope_Attack_Coeff[i]   = 1.0f;
    filterEnvelope_Decay_Coeff[i]    = 1.0f;
    filterEnvelope_Sustain_Coeff[i]  = 1.0f;
    filterEnvelope_Release_Coeff[i]  = 1.0f;
    filterEnvelope_Coefficient[i]    = 1.0f;
    savedAttackCoef[i]  = 1.0f;
    savedDecayCoef[i]   = 1.0f;
    savedSustainCoef[i] = 1.0f;
  }
}

// Helpers:
void readActiveStates(bool activeArr[]) {
  activeArr[0] = envelope_Amplitude_1.isActive();
  activeArr[1] = envelope_Amplitude_2.isActive();
  activeArr[2] = envelope_Amplitude_3.isActive();
  activeArr[3] = envelopeClone_Amplitude_1.isActive();
  activeArr[4] = envelopeClone_Amplitude_2.isActive();
  activeArr[5] = envelopeClone_Amplitude_3.isActive();
}

void readNoteOnStates(bool noteOnArr[]) {
  noteOnArr[0] = voiceActive_1;
  noteOnArr[1] = voiceActive_2;
  noteOnArr[2] = voiceActive_3;
  noteOnArr[3] = voiceActive_4;
  noteOnArr[4] = voiceActive_5;
  noteOnArr[5] = voiceActive_6;
}

void readReleasingStates(bool releasingArr[]) {
  releasingArr[0] = voiceReleasing_1;
  releasingArr[1] = voiceReleasing_2;
  releasingArr[2] = voiceReleasing_3;
  releasingArr[3] = voiceReleasing_4;
  releasingArr[4] = voiceReleasing_5;
  releasingArr[5] = voiceReleasing_6;
}

// Filter Envelope loop function
void updateEnvelopesPerVoice() {
  unsigned long now = millis();

  bool active[FILTER_ENV_VOICES];
  readActiveStates(active);             // envelope.isActive() for every voice

  bool noteOn[FILTER_ENV_VOICES];
  readNoteOnStates(noteOn);             // voiceActive_* for every voice

  bool releasing[FILTER_ENV_VOICES];
  readReleasingStates(releasing);       // voiceReleasing_* for every voice

  for (int i = 0; i < FILTER_ENV_VOICES; ++i) {
    //  RISING EDGE NOTE-ON -> re-trigger attack 
    if (!prevNoteOnArr[i] && noteOn[i]) {
      envelopeStartTime[i] = now;
      releaseStartTime[i] = 0; // cancel release if running
      filterEnvelope_Attack_Coeff[i]   = 0.0f; // attack rise 0 -> 1
      filterEnvelope_Decay_Coeff[i]    = 1.0f;
      filterEnvelope_Sustain_Coeff[i]  = 1.0f;
      filterEnvelope_Release_Coeff[i]  = 1.0f;
    }

    //  START RELEASE only on RISING EDGE voiceReleasing 
    if (releasing[i] && !prevReleasingArr[i]) {
      // save time and coefficients ONCE on release start
      releaseStartTime[i] = now;
      savedAttackCoef[i]  = filterEnvelope_Attack_Coeff[i];
      savedDecayCoef[i]   = filterEnvelope_Decay_Coeff[i];
      savedSustainCoef[i] = filterEnvelope_Sustain_Coeff[i];
    }

    //  when voiceActive true -> calculate Attack/Decay/Sustain
    if (noteOn[i]) {
      // hold note -> cancel release
      releaseStartTime[i] = 0;

      float totalElapsed = (float)(now - envelopeStartTime[i]); // ms as float

      // ATTACK 0 -> 1
      if (Filter_Attack_Value > 0.0f && totalElapsed < Filter_Attack_Value) {
        float t = totalElapsed / Filter_Attack_Value;
        filterEnvelope_Attack_Coeff[i] = t;
      } else {
        filterEnvelope_Attack_Coeff[i] = 1.0f;
      }

      // DECAY 1 -> sustain
      if (Filter_Decay_Value > 0.0f && totalElapsed > Filter_Attack_Value && totalElapsed < (Filter_Attack_Value + Filter_Decay_Value)) {
        float decayElapsed = totalElapsed - Filter_Attack_Value;
        float t = decayElapsed / Filter_Decay_Value;
        filterEnvelope_Decay_Coeff[i] = 1.0f - t * (1.0f - Filter_Sustain_Value);
      } else {
        filterEnvelope_Decay_Coeff[i] = 1.0f;
      }

      // SUSTAIN active after decay
      if ((Filter_Decay_Value > 0.0f && totalElapsed >= (Filter_Attack_Value + Filter_Decay_Value)) ||
          (Filter_Decay_Value <= 0.0f && totalElapsed >= Filter_Attack_Value)) {
        filterEnvelope_Sustain_Coeff[i] = Filter_Sustain_Value;
      } else {
        filterEnvelope_Sustain_Coeff[i] = 1.0f;
      }

      // RELEASE neutral when note is held
      filterEnvelope_Release_Coeff[i] = 1.0f;
    }
    else {
      // noteOn is not active -> calculate release if running 
      if (releaseStartTime[i] == 0) {
        // no release is active -> neutral
        filterEnvelope_Attack_Coeff[i]   = 1.0f;
        filterEnvelope_Decay_Coeff[i]    = 1.0f;
        filterEnvelope_Sustain_Coeff[i]  = 1.0f;
        filterEnvelope_Release_Coeff[i]  = 1.0f;
      } else {
        // release in progress (or finished but still in releasing state)
        float relElapsed = (float)(now - releaseStartTime[i]); // ms

        if (Filter_Release_Value > 0.0f) {
          if (relElapsed < Filter_Release_Value) {
            // dynamic: 1 -> 0 while release time not finished
            float t = relElapsed / Filter_Release_Value;
            filterEnvelope_Release_Coeff[i] = 1.0f - t;
          } else {
            // if release TIME finished, keep final release value (0.0) and keep saved
            // (so the reduction stays in effect while voiceReleasing==true)
            filterEnvelope_Release_Coeff[i] = 0.0f;
            // keep releaseStartTime[i] non-zero so we know release had started
            // (only reset everything when voiceReleasing becomes false)
          }
        } else {
          // immediate release: set to final 0.0 and keep saved until releasing==false
          filterEnvelope_Release_Coeff[i] = 0.0f;
          // leave releaseStartTime[i] as-is (it was set when release started)
        }

        // while releaseStartTime != 0 we keep the saved coefficients so the release
        // smoothly reduces from the state at note-off
        if (releaseStartTime[i] != 0) {
          filterEnvelope_Attack_Coeff[i]   = savedAttackCoef[i];
          filterEnvelope_Decay_Coeff[i]    = savedDecayCoef[i];
          filterEnvelope_Sustain_Coeff[i]  = savedSustainCoef[i];
        }
      }
    }

    // Coefficient raw product (0 .. 1)
    float rawFilterEnv =
      filterEnvelope_Attack_Coeff[i] *
      filterEnvelope_Decay_Coeff[i] *
      filterEnvelope_Sustain_Coeff[i] *
      filterEnvelope_Release_Coeff[i];

    // intensity F in -1 .. 1 - set by EG Intensity pot
    float F = Filter_Intensity_Coeff;
    F = constrain(F, -1.0f, 1.0f);

    // magnitude 0..1
    float m = fabsf(F);

    // choose target: normal raw (F>=0) or inverted raw (F<0)
    float target = (F >= 0.0f) ? rawFilterEnv : (1.0f - rawFilterEnv);

    // interpolate from neutral (1.0) toward target by amount m
    // m = 0 -> 1.0 (no effect), m = 1 -> target (full effect)
    float newFilterEnv = 1.0f - m * (1.0f - target);

    // store
    filterEnvelope_Coefficient[i] = newFilterEnv;

    // RESET: if RELEASE is on, but voiceReleasing just finished (== false),
            // we clear the states and return to the neutral state
            // -> this results in the EG coefficient staying at its final value
            // until the voiceReleasing informs of the finished releasing phase
    if (releaseStartTime[i] != 0 && !releasing[i]) {
      // fully reset
      releaseStartTime[i] = 0;
      savedAttackCoef[i]  = savedDecayCoef[i] = savedSustainCoef[i] = 1.0f;
      filterEnvelope_Attack_Coeff[i]   = 1.0f;
      filterEnvelope_Decay_Coeff[i]    = 1.0f;
      filterEnvelope_Sustain_Coeff[i]  = 1.0f;
      filterEnvelope_Release_Coeff[i]  = 1.0f;
      filterEnvelope_Coefficient[i]    = 1.0f;
    }

    //  update prev arrays 
    prevActiveArr[i] = active[i];
    prevNoteOnArr[i] = noteOn[i];
    prevReleasingArr[i] = releasing[i];
  } // end for voices
}