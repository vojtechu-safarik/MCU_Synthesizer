#ifndef DRONE_H
#define DRONE_H

#include <Arduino.h>

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
#include "Global_Variables/Wavetable_Variables.h"
// ==================

// Structure for storing a drone snapshot (settings)
struct DroneSnapshot {
    bool voiceActive[5];
    byte note[5];               // saved MIDI indexes *even with* octave offset (0..127)
    byte waveType;
    float cutoff;
    float resonance;
    float hpfFreq;
    float hpfRes;

    // AMPLITUDE ADSR
    float envAmpAttack,    envAmpDecay,    envAmpSustain,    envAmpRelease;

    // FILTER ADSR
    float envFilterAttack, envFilterDecay, envFilterSustain, envFilterRelease, envFilterIntensity;

    float savedVolume1;
    float savedVolume2;
    float savedVolume3;
    float savedTune1;
    float savedTune2;
    float savedTune3;
    float savedNoiseVolume;
    int   savedSubOctave;
    
    bool valid = false;
    unsigned long savedTimestamp = 0;

    // LFO settings (saved values, snapshot-specific)
    unsigned int lfoRate_us;   // LFOrate in microseconds (just like the global lfoRate)
    float        lfoDepth;     // 0..1
    byte         lfoWaveSelect;
    byte         lfoModeSelect;
    byte         lfoTypeSelect;
    unsigned int lfoDelay_ms;
 
    // SAVED shape parameters to reconstruct morphing exactly
    uint8_t savedWaveFamilyIndex[Drone_Wave_Voices];
    int     savedShapeIndex[Drone_Wave_Voices];

    // Snapshot params for Sequencer
    // Isolated parameters for this slot
    int   snapBPM;          
    byte  snapOctave;       
    byte  snapSeqRateSel;   
    float snapSeqGate;      // 1-100

    // Respective buffer for each slot
    bool seqEnabled = false;    
    int  seqStepCount = 0;      
    int  seqNotes[256];      // Each slot has its own 256 notes
    
};

// Structure for storing runtime voice state
struct VoiceSnapshot {
    bool active;
    byte note;               // MIDI note number for this voice (0 = unused)
    byte waveType;
    float level;
};

// GLOBAL STATE (extern)
extern DroneSnapshot droneSnapshots[8];
extern volatile bool DRONE_requestSave[8];

// wavetable results
extern int16_t droneWaveforms[Drone_Slots][Drone_Wave_Voices][TABLE_SIZE];

// filter envelope runtime
extern float droneFilterEnvelope_Coeff[Drone_Slots];
extern unsigned long droneEnvelopeStart[Drone_Slots];
extern unsigned long droneReleaseStart[Drone_Slots];
extern bool dronePrevNoteOn[Drone_Slots];
extern bool dronePrevReleasing[Drone_Slots];

extern float droneFilter_Attack_Coeff[Drone_Slots];
extern float droneFilter_Decay_Coeff[Drone_Slots];
extern float droneFilter_Sustain_Coeff[Drone_Slots];
extern float droneFilter_Release_Coeff[Drone_Slots];

extern float drone_savedAttackCoef[Drone_Slots];
extern float drone_savedDecayCoef[Drone_Slots];
extern float drone_savedSustainCoef[Drone_Slots];

// PARAMETERS / RUNTIME STATE
extern float defaultSVLpfRes;
extern float DroneA_HPFcutoff;
extern float DroneB_HPFcutoff;

// LFO state (group A / B)
extern unsigned long DroneA_LFOrate_us, DroneB_LFOrate_us;
extern float DroneA_LFOdepth, DroneB_LFOdepth;
extern int  DroneA_LFOwaveSelect, DroneB_LFOwaveSelect;
extern byte DroneA_LFOmodeSelect, DroneB_LFOmodeSelect;
extern byte DroneA_LFOtypeSelect, DroneB_LFOtypeSelect;
extern unsigned long DroneA_LFOdelay_ms, DroneB_LFOdelay_ms;
extern unsigned long DroneA_LFOdelayEndMillis, DroneB_LFOdelayEndMillis;

extern float DroneA_LFOphase, DroneB_LFOphase;
extern float DroneA_LFOonce,  DroneB_LFOonce;
extern bool  DroneA_LFOactive, DroneB_LFOactive;
extern bool  DroneA_LFOstop, DroneB_LFOstop;

// output
extern float DroneA_lfoOut, DroneB_lfoOut;

// mode / UI state
extern uint8_t DRONEmodeSelect;
extern uint8_t droneGroupActive;

extern uint8_t DRONEkeyNr1;
extern uint8_t DRONEkeyNr2;
extern uint8_t DRONEkeyNr3;
extern uint8_t DRONEkeyNr4;
extern uint8_t DRONEkeyNr5;
extern uint8_t DRONEkeyNr6;
extern uint8_t DRONEkeyNr7;
extern uint8_t DRONEkeyNr8;

extern int activeDroneA;
extern int activeDroneB;

extern bool droneGroupA_releasing;
extern bool droneGroupB_releasing;
extern int  releasingDroneA_index;
extern int  releasingDroneB_index;

extern uint8_t SeqRate;   // Speed set by potentiometer
extern float SeqGatePot;  // Gate length (0.1 - 1.0)

// API
void DroneEnvelopesInit();                              // Initialize drone envelopes
void DroneInit();                                       // Initialize drone engine

void DroneSaveValues(int slot);                         // Save drone parameter values

void DroneKeyOn(byte note);                             // Trigger drone note-on
void DroneKeyOff(byte note);                            // Trigger drone note-off
void DronePlaySlot(uint8_t slot);                       // Start drone slot playback
void DroneStopSlot(uint8_t slot);                       // Stop drone slot playback

void DroneLFO_update();                                 // Update drone LFOs
void DroneEnvelopes_update();                           // Update drone envelopes per frame

void DroneSequencer_update();

void playSnapshot(int snapIndex, bool useGroupA);       // Recall and play snapshot
void stopGroup(bool useGroupA);                         // Stop all voices in group

#endif // DRONE_H