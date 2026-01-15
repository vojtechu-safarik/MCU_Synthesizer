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

/* ================== CONFIGURATION ================== */

// Matrix size
static const byte ROWS = 3;
static const byte COLS = 3;

// Wiring
static byte rowPins[ROWS] = {30, 31, 32};
static byte colPins[COLS] = {27, 28, 29};

// Key characters (indices 0–8)
static char keymap[ROWS][COLS] = {
  {'0','1','2'},
  {'3','4','5'},
  {'6','7','8'}
};

// MIDI notes for playable keys
// (indices 0,1 and 8 are function keys and are not used for notes)
static const byte keyToMidiNote[9] = {
  0,   // 0 – octave down
  0,   // 1 – octave up
  60,  // 2
  67,  // 3
  72,  // 4
  64,  // 5
  79,  // 6 – demo / unused
  83,  // 7 – demo / unused
  0    // 8 – SHIFT
};

// Deterministic LFO wave selection per key (SHIFT + key)
// Only indices 2–7 are relevant
static const byte keyToLFOWave[9] = {
  255, // 0 – unused
  255, // 1 – unused
  0,   // 2 → LFOwaveSelect = 0
  2,   // 3 → LFOwaveSelect = 2
  3,   // 4 → LFOwaveSelect = 4
  1,   // 5 → LFOwaveSelect = 1
  255, // 6 – unused
  255, // 7 – unused
  255  // 8 – SHIFT
};

// Internal MIDI params
static constexpr byte HW_CHANNEL  = 0;
static constexpr byte HW_VELOCITY = 127;

// Index definitions
static constexpr byte KEY_OCT_DOWN   = 1;
static constexpr byte KEY_OCT_UP     = 0;
static constexpr byte KEY_FIRST_NOTE = 2;
static constexpr byte KEY_LAST_NOTE  = 7;
static constexpr byte KEY_SHIFT      = 8;

/* =================================================== */

static Keypad keypad = Keypad(
  makeKeymap(keymap),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// SHIFT state
static bool shiftActive = false;

// Octave state (0–4, default = 2)
int octaveValue = 2;

void Keyboard_update() {

  // Update keypad state
  if (!keypad.getKeys()) return;

  for (byte i = 0; i < LIST_MAX; i++) {
    if (!keypad.key[i].stateChanged) continue;

    char k = keypad.key[i].kchar;
    if (!k) continue;

    byte index = k - '0';
    if (index >= 9) continue;

    switch (keypad.key[i].kstate) {

      /* ---------- PRESSED ---------- */
      case PRESSED:

        // SHIFT key
        if (index == KEY_SHIFT) {
          shiftActive = true;
          break;
        }

        // Octave down
        if (index == KEY_OCT_DOWN) {
          if (octaveValue > 0) {
            octaveValue--;
            VirtualControlChange(0, CCoctave, octaveValue);
          }
          break;
        }

        // Octave up
        if (index == KEY_OCT_UP) {
          if (octaveValue < 4) {
            octaveValue++;
            VirtualControlChange(0, CCoctave, octaveValue);
          }
          break;
        }

        // Playable / functional keys
        if (index >= KEY_FIRST_NOTE && index <= KEY_LAST_NOTE) {

          // SHIFT = deterministic LFO wave select
          if (shiftActive) {
            byte lfoWave = keyToLFOWave[index];
            if (lfoWave != 255) {
              VirtualControlChange(0, CCLFOwaveSelect, lfoWave);
            }
          }
          // Normal note play
          else {
            byte note = keyToMidiNote[index];
            if (note != 0) {
              myNoteOn(HW_CHANNEL, note, HW_VELOCITY);
            }
          }
        }
        break;

      /* ---------- RELEASED ---------- */
      case RELEASED:

        // SHIFT release
        if (index == KEY_SHIFT) {
          shiftActive = false;
          break;
        }

        // Note off only when not in SHIFT mode
        if (!shiftActive &&
            index >= KEY_FIRST_NOTE &&
            index <= KEY_LAST_NOTE) {

          byte note = keyToMidiNote[index];
          if (note != 0) {
            myNoteOff(HW_CHANNEL, note, 0);
          }
        }
        break;

      /* ---------- HOLD / OTHER ---------- */
      default:
        break;
    }
  }
}
