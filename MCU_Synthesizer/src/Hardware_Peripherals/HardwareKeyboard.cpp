#include <Arduino.h>

// === KEYPAD lib ===
#include "Keypad.h"
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/MIDI_Config.h"
#include "Configuration\Pins_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/HardwareKeyboard.h"
// ==================

/* === CONFIGURATION === */

/* 
SeqMode: 
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

static const byte ROWS = 3;
static const byte COLS = 3;
static byte rowPins[ROWS] = {Row_1, Row_2, Row_3};
static byte colPins[COLS] = {Column_1, Column_2, Column_3};

static char keymap[ROWS][COLS] = {
  {'0','1','2'},
  {'3','4','5'},
  {'6','7','8'}
};

// MIDI notes: 254 = Rest (pause)
static const byte keyToMidiNote[9] = {
  0,   // 0 - Oct Down
  0,   // 1 - Oct Up
  60,  // 2
  67,  // 3
  72,  // 4
  64,  // 5
  79,  // 6
  83,  // 7
  254  // 8 - SHIFT / REST
};

static const byte keyToLFOWave[9] = {
  255, 255, 0, 2, 3, 1, 255, 255, 255
};

static constexpr byte HW_CHANNEL  = 0;
static constexpr byte HW_VELOCITY = 127;

static constexpr byte KEY_OCT_DOWN   = 0;
static constexpr byte KEY_OCT_UP     = 1;
static constexpr byte KEY_FIRST_NOTE = 2;
static constexpr byte KEY_LAST_NOTE  = 7;
static constexpr byte KEY_SHIFT      = 8;

unsigned long shiftPressTime = 0;

/* ================== STATE VARIABLES ================== */

static Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

static byte activeKeyStack[32]; // Amount of notes in Latch
static byte activeKeyCount = 0;
static int playingNotes[9];     // Tracking the notes in Direct Play for immediate transposition

int sequenceBuffer[256];
byte sequenceLength = 0;

static unsigned long internal_lastStepTime = 0;
static int internal_currentlyPlayingNote = -1;
static unsigned long internal_gateOffTime = 0;
static byte internal_playHead = 0; 

static bool shiftActive = false;
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
        tempNotes[i] = keyToMidiNote[activeKeyStack[i]];
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
    for(int i=0; i<9; i++) playingNotes[i] = -1;
    activeKeyCount = 0;
    internal_lastStepTime = millis();
}

void Keyboard_update() {
    // 1. Sequencer must run all the time
    internal_sequencerUpdate();

    // 2. Timer for deleteing sequence
    // must be before "if (!keypad.getKeys()) return;", so that it runs even if nothing is being pressed
    if (shiftActive && CurrentSeqMode == 2 && activeKeyCount > 0) {
        if (millis() - shiftPressTime > 2000) { 
            activeKeyCount = 0; 
            internal_stopNote();
            internal_playHead = 0;
            shiftPressTime = millis(); // Reset for next cycle
        }
    }

    // 3. Change checking for keyboard
    if (!keypad.getKeys()) return;

    for (byte i = 0; i < LIST_MAX; i++) {
        if (!keypad.key[i].stateChanged) continue;
        byte index = keypad.key[i].kchar - '0';
        if (index >= 9) continue;

        switch (keypad.key[i].kstate) {
            case PRESSED:
                if (index == KEY_OCT_DOWN) {
                    if (octaveValue > 0) {
                        octaveValue--;
                        if (CurrentSeqMode == 0) VirtualControlChange(0, CCoctave, octaveValue);
                    }
                    break;
                }
                if (index == KEY_OCT_UP) {
                    if (octaveValue < 4) {
                        octaveValue++;
                        if (CurrentSeqMode == 0) VirtualControlChange(0, CCoctave, octaveValue);
                    }
                    break;
                }
                if (index == KEY_SHIFT) {
                    shiftActive = true;
                    shiftPressTime = millis(); 
                    if (CurrentSeqMode > 0) pushKey(index); 
                    break;
                }

                if (index >= KEY_FIRST_NOTE && index <= KEY_LAST_NOTE) {
                    if (shiftActive) {
                        byte lfoWave = keyToLFOWave[index];
                        if (lfoWave != 255) VirtualControlChange(0, CCLFOwaveSelect, lfoWave);
                    } else {
                        if (CurrentSeqMode > 0) {
                            pushKey(index);
                        } else {
                            byte note = keyToMidiNote[index];
                            if (note != 0) myNoteOn(HW_CHANNEL, note, HW_VELOCITY);
                        }
                    }
                }
                break;

            case RELEASED:
                if (index == KEY_SHIFT) {
                    shiftActive = false;
                    if (CurrentSeqMode == 1) popKey(index);
                } else if (index >= KEY_FIRST_NOTE && index <= KEY_LAST_NOTE) {
                    if (CurrentSeqMode == 1) {
                        popKey(index);
                    } else if (CurrentSeqMode == 0) {
                        byte note = keyToMidiNote[index];
                        if (note != 0) myNoteOff(HW_CHANNEL, note, 0);
                    }
                }
                break;
        }
    }
}