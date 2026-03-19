#include <Arduino.h>

// === KEYPAD lib ===
#include "Keypad.h"
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/MIDI_Config.h"
#include "Configuration/Pins_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/HardwareKeyboard.h"
// ==================

/* === CONFIGURATION === */

/* SeqMode: 
    0 = Off
    1 = Arp
    2 = Latch
SeqOrder: 
    0 = Up
    1 = Down
    2 = Queue
SeqOctaves: 
    0 = 1 oct
    1 = 2 oct
    2 = 3 oct
SeqGatePot: 
    1 .. 100 
    ( how big portion of the step is the note in an active state )
SeqRateSelect:
    0 = 1/2 step length (BPM)
    1 = 1/4 step length (BPM)
    2 = 1/8 step length (BPM)
    3 = 1/16 step length (BPM)
*/

/* ===================== */

static const byte ROWS = 5;
static const byte COLS = 9;
static byte rowPins[ROWS] = {Row_1, Row_2, Row_3, Row_4, Row_5};
static byte colPins[COLS] = {Column_1, Column_2, Column_3, Column_4, Column_5, Column_6, Column_7, Column_8, Column_9};

static char keymap[ROWS][COLS] = {
  { 1,  2,  3,  4,  5,  6,  7,  8,  9},
  {10, 11, 12, 13, 14, 15, 16, 17, 18},
  {19, 20, 21, 22, 23, 24, 25, 26, 27},
  {28, 29, 30, 31, 32, 33, 34, 35, 36},
  {37, 38, 39, 40, 41, 42, 43, 44, 45}
};

// MIDI notes: 254 = Rest (pause)
static const byte keyToMidiNote[45] = {
  0,   // 0 - Oct Down
  0,   // 1 - Oct Up
  28,  // 2 - FIRST NOTE
  29,  // 3
  30,  // 4
  31,  // 5
  32,  // 6
  33,  // 7
  34,  // 8
  35,  // 9
  36,  // 10
  37,  // 11
  38,  // 12
  39,  // 13
  40,  // 14
  41,  // 15
  42,  // 16
  43,  // 17
  44,  // 18
  45,  // 19
  46,  // 20
  47,  // 21
  48,  // 22
  49,  // 23
  50,  // 24
  51,  // 25
  52,  // 26
  53,  // 27
  54,  // 28
  55,  // 29
  56,  // 30 - LAST NOTE
  254, // 31 - Rest Button
  0,   // 32 - Drone Button #1
  0,   // 33 - Drone Button #2
  0,   // 34 - Drone Button #3
  0,   // 35 - Drone Button #4
  0,   // 36 - Drone Button #5
  0,   // 37 - Drone Button #6
  0,   // 38 - Drone Button #7
  0,   // 39 - Drone Button #8
  0,   // 40 - LFO Mode #1 #2
  0,   // 41 - LFO Mode #3 #4
  0,   // 42 - (Unused)
  0,   // 43 - (Unused)
  0    // 44 - Shift Button
};

static const byte keyToLFOWave[45] = {
  255, 255,   0, 255,   1, 255,   2,   3, 255,
    4, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255
};

static const byte keyToPORTlockRate[45] = {
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255,   0,   1,   2,   3,
    4,   5, 255, 255, 255, 255, 255, 255, 255
};

static const byte keyToSeqRateSelect[45] = {
    0,   1, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255
};

static constexpr byte HW_CHANNEL  = 0;
static constexpr byte HW_VELOCITY = 127;

static constexpr byte KEY_OCT_DOWN   = 0;
static constexpr byte KEY_OCT_UP     = 1;
static constexpr byte KEY_FIRST_NOTE = 2;
static constexpr byte KEY_LAST_NOTE  = 30;
static constexpr byte KEY_REST       = 31;
static constexpr byte KEY_SHIFT      = 44;

/* ================== STATE VARIABLES ================== */

static Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

static byte activeKeyStack[32];  // Amount of notes in Latch
static byte activeKeyCount = 0;
static int playingNotes[45];     // Tracking the notes in Direct Play for immediate transposition

int sequenceBuffer[256];
byte sequenceLength = 0;

static unsigned long internal_lastStepTime = 0;
static int internal_currentlyPlayingNote = -1;
static unsigned long internal_gateOffTime = 0;
static byte internal_playHead = 0; 

static bool shiftActive = false;
static bool restActive = false;
unsigned long restPressTime = 0; // for deleting sequence

int octaveValue = 2; 

/* ================== HELPERS ================== */

void internal_stopNote() {
    if (internal_currentlyPlayingNote >= 0) {
        myNoteOff(HW_CHANNEL, (byte)internal_currentlyPlayingNote, 0);
        internal_currentlyPlayingNote = -1;
    }
}

// Immediately change the pitch of Direct play
void updateDirectPlayOctave() {
    if (CurrentSeqMode != 0) return; 
    for (int i = KEY_FIRST_NOTE; i <= KEY_LAST_NOTE; i++) {
        if (playingNotes[i] != -1) {
            myNoteOff(HW_CHANNEL, (byte)playingNotes[i], 0);
            int newNote = keyToMidiNote[i] + ((octaveValue - 2) * 12);
            if (newNote < 0) newNote = 0; if (newNote > 127) newNote = 127;
            playingNotes[i] = newNote;
            myNoteOn(HW_CHANNEL, (byte)newNote, HW_VELOCITY);
        }
    }
}

void pushKey(byte keyIndex) {
    // Arp mode (1) doesnt support duplicities
    if (CurrentSeqMode == 1) {
        for (byte i = 0; i < activeKeyCount; i++) {
            if (activeKeyStack[i] == keyIndex) return; 
        }
    }
    
    // Latch mode (2) can have the same notes in a row
    if (activeKeyCount < 12) {
        activeKeyStack[activeKeyCount++] = keyIndex;
    }
}

void popKey(byte keyIndex) {
    bool found = false;
    for (byte i = 0; i < activeKeyCount; i++) {
        if (activeKeyStack[i] == keyIndex) found = true;
        if (found && i < 11) activeKeyStack[i] = activeKeyStack[i+1];
    }
    if (found && activeKeyCount > 0) activeKeyCount--;
}

/* ================== SEQUENCER LOGIC ================== */

void rebuildSequence() {
    sequenceLength = 0;
    if (activeKeyCount == 0) return;

    int tempNotes[12];
    for (byte i = 0; i < activeKeyCount; i++) {
        if (activeKeyStack[i] == KEY_REST) {
            tempNotes[i] = 254;
        } else {
            tempNotes[i] = keyToMidiNote[activeKeyStack[i]];
        }
    }

    // Sort for UP/DOWN (Queue mode 2 is being skipped)
    if (CurrentSeqOrder < 2) {
        for (byte i = 0; i < activeKeyCount; i++) {
            for (byte j = i + 1; j < activeKeyCount; j++) {
                if (tempNotes[j] < tempNotes[i]) {
                    int t = tempNotes[i]; tempNotes[i] = tempNotes[j]; tempNotes[j] = t;
                }
            }
        }
        if (CurrentSeqOrder == 1) { // Down
            for (byte i = 0; i < activeKeyCount / 2; i++) {
                int t = tempNotes[i];
                tempNotes[i] = tempNotes[activeKeyCount - 1 - i];
                tempNotes[activeKeyCount - 1 - i] = t;
            }
        }
    }

    // Expansion + Final Buffer
    for (byte i = 0; i < activeKeyCount; i++) {
        int base = tempNotes[i];
        for (byte r = 0; r <= CurrentSeqOctave; r++) {
            if (sequenceLength < 32) {
                // If pause (254), dont increment octaves
                sequenceBuffer[sequenceLength++] = (base == 254) ? 254 : (base + (r * 12));
            }
        }
    }
}

void internal_sequencerUpdate() {
    if (CurrentSeqMode == 0) return;

    unsigned long now = millis();

    if (internal_currentlyPlayingNote >= 0 && internal_gateOffTime > 0 && now >= internal_gateOffTime) {
        internal_stopNote();
        internal_gateOffTime = 0;
    }

    int bpm = (GlobalBPM < 1) ? 120 : GlobalBPM;
    unsigned long stepMs = 60000UL / bpm; 

    if (SeqRateSelect == 0) {
        stepMs = stepMs * 2;
    } else if (SeqRateSelect == 2) {
        stepMs = stepMs / 2;
    } else if (SeqRateSelect == 3) {
        stepMs = stepMs / 4;
    }

    if ((now - internal_lastStepTime) < stepMs) return;
    internal_lastStepTime = now;

    rebuildSequence();
    if (sequenceLength == 0) {
        internal_playHead = 0;
        return;
    }

    if (internal_playHead >= sequenceLength) internal_playHead = 0;

    int rawNote = sequenceBuffer[internal_playHead];
    internal_stopNote();

    if (rawNote == 254) {
        internal_currentlyPlayingNote = -1; // pause
    } else {
        int finalNote = rawNote + ((octaveValue - 2) * 12);
        if (finalNote < 0) finalNote = 0; if (finalNote > 127) finalNote = 127;
        internal_currentlyPlayingNote = finalNote;
        myNoteOn(HW_CHANNEL, (byte)finalNote, HW_VELOCITY);
    }

    internal_gateOffTime = now + (stepMs * SeqGatePot / 100);
    internal_playHead++;
}

/* ================== CORE LOOP ================== */

void Keyboard_init() {
    for(int i=0; i<45; i++) playingNotes[i] = -1;
    activeKeyCount = 0;
    internal_lastStepTime = millis();
}

void Keyboard_update() {
    // 1. Sequencer must run all the time
    internal_sequencerUpdate();

    // 2. Timer for deleting sequence
    if (restActive && CurrentSeqMode == 2 && activeKeyCount > 0) {
        if (millis() - restPressTime > 2000) { 
            activeKeyCount = 0; 
            internal_stopNote();
            internal_playHead = 0;
            restPressTime = millis(); // Reset for next cycle
        }
    }

    // 3. Change checking for keyboard
    if (!keypad.getKeys()) return;

    for (byte i = 0; i < LIST_MAX; i++) {
        if (!keypad.key[i].stateChanged) continue;
        
        byte index = keypad.key[i].kchar - 1; 
        if (index >= 45) continue;

        switch (keypad.key[i].kstate) {
            case PRESSED:
                if (index == KEY_SHIFT) {
                    shiftActive = true;
                    break;
                }

                // --- SHIFT LOGIC ---
                if (shiftActive) {
                    // LFO Wave selection
                    if (keyToLFOWave[index] != 255) {
                        VirtualControlChange(0, CC_LFOwaveSelect, keyToLFOWave[index]);
                    }
                    // Portamento/Lock Rate
                    if (keyToPORTlockRate[index] != 255) {
                        VirtualControlChange(0, CC_PORTlockRate, keyToPORTlockRate[index]);
                    }
                    
                    // Seq Rate Increment/Decrement (0..3)
                    // KEY_OCT_DOWN (Index 0) decreases rate
                    if (index == KEY_OCT_DOWN) {
                        if (SeqRateSelect > 0) SeqRateSelect--;
                    }
                    // KEY_OCT_UP (Index 1) increases rate
                    if (index == KEY_OCT_UP) {
                        if (SeqRateSelect < 3) SeqRateSelect++;
                    }
                    break; 
                }

                // --- NORMAL LOGIC ---
                if (index == KEY_OCT_DOWN) {
                    if (octaveValue > 0) {
                        octaveValue--;
                        if (CurrentSeqMode == 0) VirtualControlChange(0, CC_octave, octaveValue);
                    }
                    break;
                }
                if (index == KEY_OCT_UP) {
                    if (octaveValue < 4) {
                        octaveValue++;
                        if (CurrentSeqMode == 0) VirtualControlChange(0, CC_octave, octaveValue);
                    }
                    break;
                }

                if (index == KEY_REST) {
                    restActive = true;
                    restPressTime = millis();
                    if (CurrentSeqMode > 0) pushKey(index);
                    break;
                }

                if (index >= KEY_FIRST_NOTE && index <= KEY_LAST_NOTE) {
                    if (CurrentSeqMode > 0) {
                        pushKey(index);
                    } else {
                        byte note = keyToMidiNote[index];
                        if (note != 0) {
                            playingNotes[index] = note + ((octaveValue - 2) * 12);
                            myNoteOn(HW_CHANNEL, (byte)playingNotes[index], HW_VELOCITY);
                        }
                    }
                }
                break;

            case RELEASED:
                if (index == KEY_SHIFT) {
                    shiftActive = false;
                } 
                else if (index == KEY_REST) {
                    restActive = false;
                    if (CurrentSeqMode == 1) popKey(index);
                } 
                else if (index >= KEY_FIRST_NOTE && index <= KEY_LAST_NOTE) {
                    if (CurrentSeqMode == 1) {
                        popKey(index);
                    } else if (CurrentSeqMode == 0) {
                        byte note = keyToMidiNote[index];
                        if (note != 0 && playingNotes[index] != -1) {
                            myNoteOff(HW_CHANNEL, (byte)playingNotes[index], 0);
                            playingNotes[index] = -1;
                        }
                    }
                }
                break;
        }
    }
}