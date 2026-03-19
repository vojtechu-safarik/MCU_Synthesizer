// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
#include "Global_Variables/Wavetable_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis/Teensy_Audio.h"
#include "Synthesis/Portamento.h" // smazat, možná zbytečný
#include "Synthesis/Drone.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/MIDI_Config.h"
// ==================

// Storage
DroneSnapshot droneSnapshots[8];
volatile bool DRONE_requestSave[8] = { false, false, false, false, false, false, false, false };

int16_t droneWaveforms[Drone_Slots][Drone_Wave_Voices][TABLE_SIZE];

// Envelope state
float droneFilterEnvelope_Coeff[Drone_Slots];
unsigned long droneEnvelopeStart[Drone_Slots];
unsigned long droneReleaseStart[Drone_Slots];
bool dronePrevNoteOn[Drone_Slots];
bool dronePrevReleasing[Drone_Slots];

float droneFilter_Attack_Coeff[Drone_Slots];
float droneFilter_Decay_Coeff[Drone_Slots];
float droneFilter_Sustain_Coeff[Drone_Slots];
float droneFilter_Release_Coeff[Drone_Slots];

float drone_savedAttackCoef[Drone_Slots];
float drone_savedDecayCoef[Drone_Slots];
float drone_savedSustainCoef[Drone_Slots];

// Parameters
float defaultSVLpfRes = 0.707f;
float DroneA_HPFcutoff = HPFcutoffFreq;
float DroneB_HPFcutoff = HPFcutoffFreq;

// LFO State
unsigned long DroneA_LFOrate_us = 0;
unsigned long DroneB_LFOrate_us = 0;

float DroneA_LFOdepth = 0.0f;
float DroneB_LFOdepth = 0.0f;

int DroneA_LFOwaveSelect = 0;
int DroneB_LFOwaveSelect = 0;

byte DroneA_LFOmodeSelect = 0;
byte DroneB_LFOmodeSelect = 0;

byte DroneA_LFOtypeSelect = 0;
byte DroneB_LFOtypeSelect = 0;

unsigned long DroneA_LFOdelay_ms = 0;
unsigned long DroneB_LFOdelay_ms = 0;

unsigned long DroneA_LFOdelayEndMillis = 0;
unsigned long DroneB_LFOdelayEndMillis = 0;

float DroneA_LFOphase = 0.0f;
float DroneB_LFOphase = 0.0f;

float DroneA_LFOonce = 1.0f;
float DroneB_LFOonce = 1.0f;

bool DroneA_LFOactive = false;
bool DroneB_LFOactive = false;

bool DroneA_LFOstop = false;
bool DroneB_LFOstop = false;

float DroneA_lfoOut = 0.0f;
float DroneB_lfoOut = 0.0f;

// MODE / UI
uint8_t DRONEmodeSelect = 0;
uint8_t droneGroupActive = 0;

uint8_t DRONEkeyNr1 = 0;
uint8_t DRONEkeyNr2 = 0;
uint8_t DRONEkeyNr3 = 0;
uint8_t DRONEkeyNr4 = 0;
uint8_t DRONEkeyNr5 = 0;
uint8_t DRONEkeyNr6 = 0;
uint8_t DRONEkeyNr7 = 0;
uint8_t DRONEkeyNr8 = 0;

int activeDroneA = -1;
int activeDroneB = -1;

bool droneGroupA_releasing = false;
bool droneGroupB_releasing = false;
int releasingDroneA_index = -1;
int releasingDroneB_index = -1;

// Functions
void DroneInit() {
    DRONEmodeSelect = 0;

    DRONEkeyNr1 = DRONEkeyNr2 = DRONEkeyNr3 = DRONEkeyNr4 =
    DRONEkeyNr5 = DRONEkeyNr6 = DRONEkeyNr7 = DRONEkeyNr8 = 0;
}

void DroneEnvelopesInit() {
    for (int i = 0; i < Drone_Slots; ++i) {
        droneFilterEnvelope_Coeff[i] = 1.0f;
        droneEnvelopeStart[i] = 0;
        droneReleaseStart[i] = 0;
        dronePrevNoteOn[i] = false;
        dronePrevReleasing[i] = false;

        droneFilter_Attack_Coeff[i]  = 1.0f;
        droneFilter_Decay_Coeff[i]   = 1.0f;
        droneFilter_Sustain_Coeff[i] = 1.0f;
        droneFilter_Release_Coeff[i] = 1.0f;

        drone_savedAttackCoef[i]  = 1.0f;
        drone_savedDecayCoef[i]   = 1.0f;
        drone_savedSustainCoef[i] = 1.0f;
    }
}