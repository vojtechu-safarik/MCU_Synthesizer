#include <Arduino.h>

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// Wavetable state variables
int currentWaveFamily_1 = 0;    // index of active wavefamily
int currentShapeIndex_1 = 0;    // morph steps in current wavefamily (0 .. morphSteps*(numWaves-1))
int currentWaveFamily_2 = 0;
int currentShapeIndex_2 = 0;
int currentWaveFamily_3 = 0;
int currentShapeIndex_3 = 0;

// Default filter parameters
float LPFcutoffFreq = 10000.0f;
float HPFcutoffFreq = 20.0f;
float LPFresonanceVal = 0.0f;
float LPFcutoffValue = 1.0f;
float defaultHpfFreq = 20.0f;
float defaultHpfRes  = 0.707f;
// Default volume
float defaultVolume_1 = 0.75f;
float defaultVolume_2 = 0.75f;
float defaultVolume_3 = 0.75f;
float pinkVolume = 0.00f;
// Master volume
float MASTERVolume = 1.0f;

// Global Variables of Keyboard
const byte Buffer = 8; // Size of keyboard buffer
const float noteFreqs[128] = {8.176,    8.662,    9.177,    9.723,   10.301,   10.913,   11.562,    12.25, 
                             12.978,    13.75,   14.568,   15.434,   16.352,   17.324,   18.354,   19.445, 
                             20.602,   21.827,   23.125,     24.5,   25.957,     27.5,   29.135,   30.868, 
                             32.703,   34.648,   36.708,   38.891,   41.203,   43.654,   46.249,   48.999, 
                             51.913,       55,    58.27,   61.735,   65.406,   69.296,   73.416,   77.782, 
                             82.407,   87.307,   92.499,   97.999,  103.826,      110,  116.541,  123.471, 
                            130.813,  138.591,  146.832,  155.563,  164.814,  174.614,  184.997,  195.998, 
                            207.652,      220,  233.082,  246.942,  261.626,  277.183,  293.665,  311.127, 
                            329.628,  349.228,  369.994,  391.995,  415.305,      440,  466.164,  493.883, 
                            523.251,  554.365,   587.33,  622.254,  659.255,  698.456,  739.989,  783.991, 
                            830.609,      880,  932.328,  987.767, 1046.502, 1108.731, 1174.659, 1244.508, 
                            1318.51, 1396.913, 1479.978, 1567.982, 1661.219,     1760, 1864.655, 1975.533, 
                           2093.005, 2217.461, 2349.318, 2489.016,  2637.02, 2793.826, 2959.955, 3135.963, 
                           3322.438,     3520,  3729.31, 3951.066, 4186.009, 4434.922, 4698.636, 4978.032, 
                           5274.041, 5587.652, 5919.911, 6271.927, 6644.875,     7040,  7458.62, 7902.133, 
                           8372.018, 8869.844, 9397.273, 9956.063, 10548.08,  11175.3, 11839.82, 12543.85};
// byte globalVelocity = 1; // not used in this version
int octave = 0;
int SubOctave = 0;

// Global Variables of Synth mode
byte SynthMode = 0;   // Unison / Poly
bool voiceActive_1 = false;
bool voiceActive_2 = false;
bool voiceActive_3 = false;
bool voiceActive_4 = false;
bool voiceActive_5 = false;
bool voiceActive_6 = false;
bool voiceActive_SUB = false;
float voiceFreq_1 = 0.0f;
float voiceFreq_2 = 0.0f;
float voiceFreq_3 = 0.0f;
float voiceFreq_4 = 0.0f;
float voiceFreq_5 = 0.0f;
float voiceFreq_6 = 0.0f;
byte voiceNote_1 = 0;
byte voiceNote_2 = 0;
byte voiceNote_3 = 0;
byte voiceNote_4 = 0;
byte voiceNote_5 = 0;
byte voiceNote_6 = 0;

// Global Variables of LFO
unsigned int LFOrate = 10000000;  // Speed (time in microseconds)
float LFOdepth = 0;               // The modulation depth (0–1)
float LFOdelay = 0;
int LFOwaveSelect = 0;            // index of active LFO Wavetable
byte LFOmodeSelect = 0;
byte LFOtypeSelect = 0;

// per-voice LFO delay management (microseconds end time + activation)
unsigned long LFOdelayEnd_1 = 0;
unsigned long LFOdelayEnd_2 = 0;
unsigned long LFOdelayEnd_3 = 0;
unsigned long LFOdelayEnd_4 = 0;
unsigned long LFOdelayEnd_5 = 0;
unsigned long LFOdelayEnd_6 = 0;
bool LFOdelaying_1 = false;
bool LFOdelaying_2 = false;
bool LFOdelaying_3 = false;
bool LFOdelaying_4 = false;
bool LFOdelaying_5 = false;
bool LFOdelaying_6 = false;
float lfoOut_1 = 0.0f;
float lfoOut_2 = 0.0f;
float lfoOut_3 = 0.0f;
float lfoOut_4 = 0.0f;
float lfoOut_5 = 0.0f;
float lfoOut_6 = 0.0f;
float LFOphase_1 = 0.0f; // time in seconds in within the period
float LFOphase_2 = 0.0f;
float LFOphase_3 = 0.0f;
float LFOphase_4 = 0.0f;
float LFOphase_5 = 0.0f;
float LFOphase_6 = 0.0f;
float LFOglobalPhase = 0.0f; // synced LFO for Unison Triads
float LFOonce_1 = 1.0f;
float LFOonce_2 = 1.0f;
float LFOonce_3 = 1.0f;
float LFOonce_4 = 1.0f;
float LFOonce_5 = 1.0f;
float LFOonce_6 = 1.0f;
bool LFOactive_1 = false;
bool LFOactive_2 = false;
bool LFOactive_3 = false;
bool LFOactive_4 = false;
bool LFOactive_5 = false;
bool LFOactive_6 = false;
bool voiceReleasing_1 = false;
bool voiceReleasing_2 = false;
bool voiceReleasing_3 = false;
bool voiceReleasing_4 = false;
bool voiceReleasing_5 = false;
bool voiceReleasing_6 = false;
bool gate_1 = false;
bool gate_2 = false;
bool gate_3 = false;
bool gate_4 = false;
bool gate_5 = false;
bool gate_6 = false;
bool lastGate_1 = false;
bool lastGate_2 = false;
bool lastGate_3 = false;
bool lastGate_4 = false;
bool lastGate_5 = false;
bool lastGate_6 = false;
bool LFOstop_1 = false;   // true = stop rise/fall
bool LFOstop_2 = false;
bool LFOstop_3 = false;
bool LFOstop_4 = false;
bool LFOstop_5 = false;
bool LFOstop_6 = false;
elapsedMicros lfoTimer;   // timer for LFO update

float LFOfreqCutoffMod_1 = 1.0f;
float LFOfreqCutoffMod_2 = 1.0f;
float LFOfreqCutoffMod_3 = 1.0f;
float LFOfreqCutoffMod_4 = 1.0f;
float LFOfreqCutoffMod_5 = 1.0f;
float LFOfreqCutoffMod_6 = 1.0f;
float LFOfreqCutoffMod_7 = 1.0f; // sub-oscillator

float Tune_1 = 0.5f;
float Tune_2 = 0.5f;
float Tune_3 = 0.5f;
float Tune_4 = 0.5f;
float Tune_5 = 0.5f;
float Tune_6 = 0.5f;
float LFOTune_1 = 0.5f;
float LFOTune_2 = 0.5f;
float LFOTune_3 = 0.5f;
float LFOTune_4 = 0.5f;
float LFOTune_5 = 0.5f;
float LFOTune_6 = 0.5f;

// ====== global variables of Filter Envelope + helper functions ======
float Filter_Attack_Value  = 0.0f; // ms (0..3000)
float Filter_Decay_Value   = 0.0f; // ms (0..3000)
float Filter_Sustain_Value = 1.0f; // 0..1
float Filter_Release_Value = 500.0f; // ms (0..3000)
float Filter_Intensity_Coeff = 1.0f; // 0..1 (0 neutralizes the Filter EG, 1 keeps it at full intensity)

// 0 = Default triad, 1 = Clone triad
uint8_t unisonTriad = 0;
bool unisonTriadToggle = false;
float triadBaseFreq[2] = {0, 0};   // default frequency for triad 0 and 1

// Global Variables of LOCK mode
int GlobalBPM = 120;
byte EGlockSelect = 0;
byte LFOlockSelect = 0;
byte EGlockRateSelect = 0;
byte LFOlockRateSelect = 0;

// LOCK mapping: index 0..5 (0=2/1, 5=1/8)
const float LOCK_FACTORS_BY_INDEX[6] = {
  8.0f,        // 0 -> 2/1  (double whole = 8 * quarter)
  4.0f,        // 1 -> 1/1  (whole = 4 * quarter)
  2.0f,        // 2 -> 1/2  (half = 2 * quarter)
  1.0f,        // 3 -> 1/4  (quarter = 1 * quarter)
  2.0f/3.0f,   // 4 -> 1/4T (quarter triplet = 2/3 * quarter)
  0.5f         // 5 -> 1/8  (eighth = 1/2 * quarter)
};

float KBDtrackingMod_1 = 1.0f;
float KBDtrackingMod_2 = 1.0f;
float KBDtrackingMod_3 = 1.0f;
float KBDtrackingMod_4 = 1.0f;
float KBDtrackingMod_5 = 1.0f;
float KBDtrackingMod_6 = 1.0f;
float KBDtrackingMod_7 = 1.0f; // sub-oscillator

// --- Keyboard tracking (static base = C3) ---
const int KBD_BASE_NOTE = 48; // C3 (MIDI note 48)
float KBDtracking = 0.0f;     // 0 ... 1
float KBDmult[128];           // pre-calculated multiplicators (2^((n-base)/12))

// Global Variables of PORTAMENTO
byte PORTswitch = 0;
byte PORTmodeSelect = 0;
byte PORTlockRate = 0;
// Current state in semitones (log-frequency)
float PORTcurrentSemitone_1 = 0.0f;
float PORTcurrentSemitone_2 = 0.0f; 
float PORTcurrentSemitone_3 = 0.0f;
float PORTcurrentSemitone_4 = 0.0f; 
float PORTcurrentSemitone_5 = 0.0f; 
float PORTcurrentSemitone_6 = 0.0f;
float PORTtargetSemitone_1 = 0.0f;
float PORTtargetSemitone_2 = 0.0f; 
float PORTtargetSemitone_3 = 0.0f;
float PORTtargetSemitone_4 = 0.0f; 
float PORTtargetSemitone_5 = 0.0f; 
float PORTtargetSemitone_6 = 0.0f;
float PORTstartSemitone_1  = 0.0f; 
float PORTstartSemitone_2  = 0.0f; 
float PORTstartSemitone_3  = 0.0f;
float PORTstartSemitone_4  = 0.0f; 
float PORTstartSemitone_5  = 0.0f; 
float PORTstartSemitone_6  = 0.0f;
float PORTonce_1 = 1.0f;
float PORTonce_2 = 1.0f;
float PORTonce_3 = 1.0f;
float PORTonce_4 = 1.0f;
float PORTonce_5 = 1.0f;
float PORTonce_6 = 1.0f;
bool PORTactive_1 = false;
bool PORTactive_2 = false;
bool PORTactive_3 = false;
bool PORTactive_4 = false;
bool PORTactive_5 = false;
bool PORTactive_6 = false;
bool PORTstop_1 = true;
bool PORTstop_2 = true;
bool PORTstop_3 = true;
bool PORTstop_4 = true;
bool PORTstop_5 = true;
bool PORTstop_6 = true;
float PORTdetune_1 = 1.0f;
float PORTdetune_2 = 1.0f;
float PORTdetune_3 = 1.0f;
float PORTdetune_4 = 1.0f;
float PORTdetune_5 = 1.0f;
float PORTdetune_6 = 1.0f;
float portTimeControl = 1.0f;                    // 0 ... 1 fraction of subdivision for Fixed Time
float portRateSemitonesPerSubdivision = 12.0f;   // default: 12 semitones per subdivision
// internal timer
unsigned long lastPortamentoMicros = 0;

// PORTAMENTO LOCK mapping: index 0 ... 6
const float PORTAMENTO_LOCK_FACTORS_BY_INDEX[7] = {
 16.0f,        // 0 -> 4/1
  8.0f,        // 0 -> 2/1
  4.0f,        // 1 -> 1/1
  2.0f,        // 2 -> 1/2
  1.0f,        // 3 -> 1/4
  0.5f,        // 4 -> 1/8
 0.25f         // 5 -> 1/16
};

// Global Variables of Drone mode
float DroneCarry_Amplitude_Attack_Value = 0.0f;
float DroneCarry_Amplitude_Decay_Value = 0.0f;
float DroneCarry_Amplitude_Sustain_Value = 1.0f;
float DroneCarry_Amplitude_Release_Value = 500.0f;
float DroneCarry_Filter_Attack_Value = 0.0f;
float DroneCarry_Filter_Decay_Value = 0.0f;
float DroneCarry_Filter_Sustain_Value = 1.0f;
float DroneCarry_Filter_Release_Value = 500.0f;
float DroneCarry_Filter_Intensity_Coeff = 1.0f; // 0..1 (0 neutralizes the Filter EG, 1 keeps it at full intensity)

float DroneA_finalCutoff1 = 10000.0f;
float DroneA_finalCutoff2 = 10000.0f;
float DroneA_finalCutoff3 = 10000.0f;
float DroneA_finalCutoff4 = 10000.0f;
float DroneA_finalCutoff5 = 10000.0f;
float DroneA_finalCutoff6 = 10000.0f;
float DroneA_finalCutoff70 = 10000.0f;
float DroneA_finalCutoff71 = 10000.0f;
float DroneB_finalCutoff1 = 10000.0f;
float DroneB_finalCutoff2 = 10000.0f;
float DroneB_finalCutoff3 = 10000.0f;
float DroneB_finalCutoff4 = 10000.0f;
float DroneB_finalCutoff5 = 10000.0f;
float DroneB_finalCutoff6 = 10000.0f;
float DroneB_finalCutoff70 = 10000.0f;
float DroneB_finalCutoff71 = 10000.0f;

const float DIV127 = (1.0 / 127.0);  // normalises the MIDI range (127 values correspond to 0-1)