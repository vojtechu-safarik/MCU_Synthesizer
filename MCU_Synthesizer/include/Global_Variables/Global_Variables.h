#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <Arduino.h>

extern int currentWaveFamily_1;
extern int currentShapeIndex_1;
extern int currentWaveFamily_2;
extern int currentShapeIndex_2;
extern int currentWaveFamily_3;
extern int currentShapeIndex_3;

extern float LPFcutoffFreq;
extern float HPFcutoffFreq;
extern float LPFresonanceVal;
extern float LPFcutoffValue;
extern float defaultHpfFreq;
extern float defaultHpfRes;

extern float defaultVolume_1;
extern float defaultVolume_2;
extern float defaultVolume_3;
extern float pinkVolume;

extern float MASTERVolume;

extern const byte Buffer;
extern const float noteFreqs[128];
extern int octave;
extern int SubOctave;

extern byte SynthMode;
extern bool voiceActive_1;
extern bool voiceActive_2;
extern bool voiceActive_3;
extern bool voiceActive_4;
extern bool voiceActive_5;
extern bool voiceActive_6;
extern bool voiceActive_SUB;
extern float voiceFreq_1;
extern float voiceFreq_2;
extern float voiceFreq_3;
extern float voiceFreq_4;
extern float voiceFreq_5;
extern float voiceFreq_6;
extern byte voiceNote_1;
extern byte voiceNote_2;
extern byte voiceNote_3;
extern byte voiceNote_4;
extern byte voiceNote_5;
extern byte voiceNote_6;

extern unsigned int LFOrate;
extern float LFOdepth;
extern float LFOdelay;
extern int LFOwaveSelect;
extern byte LFOmodeSelect;
extern byte LFOtypeSelect;

extern unsigned long LFOdelayEnd_1;
extern unsigned long LFOdelayEnd_2;
extern unsigned long LFOdelayEnd_3;
extern unsigned long LFOdelayEnd_4;
extern unsigned long LFOdelayEnd_5;
extern unsigned long LFOdelayEnd_6;
extern bool LFOdelaying_1;
extern bool LFOdelaying_2;
extern bool LFOdelaying_3;
extern bool LFOdelaying_4;
extern bool LFOdelaying_5;
extern bool LFOdelaying_6;
extern float lfoOut_1;
extern float lfoOut_2;
extern float lfoOut_3;
extern float lfoOut_4;
extern float lfoOut_5;
extern float lfoOut_6;
extern float LFOphase_1;
extern float LFOphase_2;
extern float LFOphase_3;
extern float LFOphase_4;
extern float LFOphase_5;
extern float LFOphase_6;
extern float LFOglobalPhase;
extern float LFOonce_1;
extern float LFOonce_2;
extern float LFOonce_3;
extern float LFOonce_4;
extern float LFOonce_5;
extern float LFOonce_6;
extern bool LFOactive_1;
extern bool LFOactive_2;
extern bool LFOactive_3;
extern bool LFOactive_4;
extern bool LFOactive_5;
extern bool LFOactive_6;
extern bool voiceReleasing_1;
extern bool voiceReleasing_2;
extern bool voiceReleasing_3;
extern bool voiceReleasing_4;
extern bool voiceReleasing_5;
extern bool voiceReleasing_6;
extern bool gate_1;
extern bool gate_2;
extern bool gate_3;
extern bool gate_4;
extern bool gate_5;
extern bool gate_6;
extern bool lastGate_1;
extern bool lastGate_2;
extern bool lastGate_3;
extern bool lastGate_4;
extern bool lastGate_5;
extern bool lastGate_6;
extern bool LFOstop_1;
extern bool LFOstop_2;
extern bool LFOstop_3;
extern bool LFOstop_4;
extern bool LFOstop_5;
extern bool LFOstop_6;
extern elapsedMicros lfoTimer;

extern float LFOfreqCutoffMod_1;
extern float LFOfreqCutoffMod_2;
extern float LFOfreqCutoffMod_3;
extern float LFOfreqCutoffMod_4;
extern float LFOfreqCutoffMod_5;
extern float LFOfreqCutoffMod_6;
extern float LFOfreqCutoffMod_7;

extern float Tune_1;
extern float Tune_2;
extern float Tune_3;
extern float Tune_4;
extern float Tune_5;
extern float Tune_6;
extern float LFOTune_1;
extern float LFOTune_2;
extern float LFOTune_3;
extern float LFOTune_4;
extern float LFOTune_5;
extern float LFOTune_6;

extern float Filter_Attack_Value;
extern float Filter_Decay_Value;
extern float Filter_Sustain_Value;
extern float Filter_Release_Value;
extern float Filter_Intensity_Coeff;

extern uint8_t unisonTriad;
extern bool unisonTriadToggle;
extern float triadBaseFreq[2];

extern int GlobalBPM;
extern byte EGlockSelect;
extern byte LFOlockSelect;
extern byte EGlockRateSelect;
extern byte LFOlockRateSelect;

extern const float LOCK_FACTORS_BY_INDEX[6];

extern float KBDtrackingMod_1;
extern float KBDtrackingMod_2;
extern float KBDtrackingMod_3;
extern float KBDtrackingMod_4;
extern float KBDtrackingMod_5;
extern float KBDtrackingMod_6;
extern float KBDtrackingMod_7;

extern const int KBD_BASE_NOTE;
extern float KBDtracking;
extern float KBDmult[128];

extern byte PORTswitch;
extern byte PORTmodeSelect;
extern byte PORTlockRate;
extern float PORTcurrentSemitone_1;
extern float PORTcurrentSemitone_2;
extern float PORTcurrentSemitone_3;
extern float PORTcurrentSemitone_4;
extern float PORTcurrentSemitone_5;
extern float PORTcurrentSemitone_6;
extern float PORTtargetSemitone_1;
extern float PORTtargetSemitone_2;
extern float PORTtargetSemitone_3;
extern float PORTtargetSemitone_4;
extern float PORTtargetSemitone_5;
extern float PORTtargetSemitone_6;
extern float PORTstartSemitone_1;
extern float PORTstartSemitone_2;
extern float PORTstartSemitone_3;
extern float PORTstartSemitone_4;
extern float PORTstartSemitone_5;
extern float PORTstartSemitone_6;
extern float PORTonce_1;
extern float PORTonce_2;
extern float PORTonce_3;
extern float PORTonce_4;
extern float PORTonce_5;
extern float PORTonce_6;
extern bool PORTactive_1;
extern bool PORTactive_2;
extern bool PORTactive_3;
extern bool PORTactive_4;
extern bool PORTactive_5;
extern bool PORTactive_6;
extern bool PORTstop_1;
extern bool PORTstop_2;
extern bool PORTstop_3;
extern bool PORTstop_4;
extern bool PORTstop_5;
extern bool PORTstop_6;
extern float PORTdetune_1;
extern float PORTdetune_2;
extern float PORTdetune_3;
extern float PORTdetune_4;
extern float PORTdetune_5;
extern float PORTdetune_6;
extern float portTimeControl;
extern float portRateSemitonesPerSubdivision;
extern unsigned long lastPortamentoMicros;

extern const float PORTAMENTO_LOCK_FACTORS_BY_INDEX[7];

// ==== DRONE ====
// Drone constants
const int DRONE_SLOTS = 8;
const int DRONE_WAVE_VOICES = 3;

extern float DroneCarry_Amplitude_Attack_Value;
extern float DroneCarry_Amplitude_Decay_Value;
extern float DroneCarry_Amplitude_Sustain_Value;
extern float DroneCarry_Amplitude_Release_Value;
extern float DroneCarry_Filter_Attack_Value;
extern float DroneCarry_Filter_Decay_Value;
extern float DroneCarry_Filter_Sustain_Value;
extern float DroneCarry_Filter_Release_Value;
extern float DroneCarry_Filter_Intensity_Coeff;

extern float DroneA_finalCutoff1;
extern float DroneA_finalCutoff2;
extern float DroneA_finalCutoff3;
extern float DroneA_finalCutoff4;
extern float DroneA_finalCutoff5;
extern float DroneA_finalCutoff6;
extern float DroneA_finalCutoff70;
extern float DroneA_finalCutoff71;
extern float DroneB_finalCutoff1;
extern float DroneB_finalCutoff2;
extern float DroneB_finalCutoff3;
extern float DroneB_finalCutoff4;
extern float DroneB_finalCutoff5;
extern float DroneB_finalCutoff6;
extern float DroneB_finalCutoff70;
extern float DroneB_finalCutoff71;

extern const float DIV127;

#endif // GLOBAL_VARIABLES_H