#ifndef FILTER_ENVELOPE_H
#define FILTER_ENVELOPE_H

// Constant
const int FILTER_ENV_VOICES = 6;

// EXTERN VARIABLES
extern unsigned long envelopeStartTime[FILTER_ENV_VOICES];
extern unsigned long releaseStartTime[FILTER_ENV_VOICES];

extern bool prevActiveArr[FILTER_ENV_VOICES]; // previous active state per voice
extern bool prevNoteOnArr[FILTER_ENV_VOICES]; // track previous voiceActive state (note-on edge)
extern bool prevReleasingArr[FILTER_ENV_VOICES]; // track previous voiceReleasing state (release edge)

extern float filterEnvelope_Attack_Coeff[FILTER_ENV_VOICES];
extern float filterEnvelope_Decay_Coeff[FILTER_ENV_VOICES];
extern float filterEnvelope_Sustain_Coeff[FILTER_ENV_VOICES];
extern float filterEnvelope_Release_Coeff[FILTER_ENV_VOICES];
extern float filterEnvelope_Coefficient[FILTER_ENV_VOICES]; // Result coefficient for every voice

// Release continues dynamically during note-off
extern float savedAttackCoef[FILTER_ENV_VOICES];
extern float savedDecayCoef[FILTER_ENV_VOICES];
extern float savedSustainCoef[FILTER_ENV_VOICES];

// FUNCTIONS
void readActiveStates(bool activeArr[]);                // Read active voice states
void readNoteOnStates(bool noteOnArr[]);                // Read note-on states
void readReleasingStates(bool releasingArr[]);          // Read envelope release states
void initEnvelopesPerVoice();                           // Initialize envelopes for all voices
void updateEnvelopesPerVoice();                         // Update envelopes for each voice

#endif // FILTER_ENVELOPE_H