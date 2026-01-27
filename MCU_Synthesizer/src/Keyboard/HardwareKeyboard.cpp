#include <Arduino.h>

// === KEYPAD lib ===
#include "Keypad.h"
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ==================

// === CONTROLS ===
#include "Controls/MIDI_Control.h"
// ==================

// === KEYBOARD ===
#include "Keyboard/HardwareKeyboard.h"
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
*/

/* ===================== */

static const byte ROWS = 3;
static const byte COLS = 3;
static byte rowPins[ROWS] = {30, 31, 32};
static byte colPins[COLS] = {27, 28, 29};

static char keymap[ROWS][COLS] = {
  {'0','1','2'},
  {'3','4','5'},
  {'6','7','8'}
};

// MIDI notes: 254 = Rest (pauza)
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

static byte activeKeyStack[32]; // Zvětšeno pro více not v Latch/Range
static byte activeKeyCount = 0;
static int playingNotes[9];     // Sledování not v Direct Play pro okamžitou transpozici

static int sequenceBuffer[256];
static byte sequenceLength = 0;

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

// Okamžitě změní výšku tónů, které právě zní v Direct Play
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
    // V Arp módu (1) duplicity nechceme, aby to nehrálo divně
    if (CurrentSeqMode == 1) {
        for (byte i = 0; i < activeKeyCount; i++) {
            if (activeKeyStack[i] == keyIndex) return; 
        }
    }
    
    // V Latch módu (2) a ostatních teď můžeš sázet stejné noty za sebe
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

    // Sort pro UP/DOWN (Queue mód 2 se přeskakuje)
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
                // Pokud je to pauza (254), nepřičítáme oktávy
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
        internal_currentlyPlayingNote = -1; // Pauza
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
    // 1. Sequencer musí běžet pořád
    internal_sequencerUpdate();

    // 2. TIMER PRO SMAZÁNÍ (přesunuto SEM)
    // Musí být před "if (!keypad.getKeys()) return;", aby běžel i když se na nic nemačká
    if (shiftActive && CurrentSeqMode == 2 && activeKeyCount > 0) {
        if (millis() - shiftPressTime > 2000) { 
            activeKeyCount = 0; 
            internal_stopNote();
            internal_playHead = 0;
            shiftPressTime = millis(); // Reset pro další cyklus
        }
    }

    // 3. Kontrola změn na klávesnici
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