#include <Arduino.h>

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ========================

// === KEYBOARD ===
#include "Keyboard/LEDoctave.h"
// ==================

// === DRIVERS ===
#include "Drivers/IO_Expander.h"
// ==================

// LED mapping on PCF pins
static constexpr byte LED_OCT_1 = 0;
static constexpr byte LED_OCT_2 = 1;
static constexpr byte LED_OCT_3 = 2;
static constexpr byte LED_OCT_4 = 3;

// Total number of octave states (0–4)
static constexpr byte OCTAVE_MIN = 0;
static constexpr byte OCTAVE_MAX = 4;

// Neutral octave (no LEDs on)
static constexpr byte OCTAVE_CENTER = 2;

// ========================

/**
 * @brief Initialize octave LEDs
 * Call once in setup()
 */
void LEDoctave_begin() {
  // Nothing hardware-specific here.
  // Ensure IO Expander is initialized separately by calling IOExpander_begin() in setup().
}


/**
 * @brief Update octave LEDs according to octaveValue
 * Call regularly from loop()
 */
void LEDoctave_update() {

  // Safety clamp (in case octaveValue is modified elsewhere)
  int octave = octaveValue;
  if (octave < OCTAVE_MIN) octave = OCTAVE_MIN;
  if (octave > OCTAVE_MAX) octave = OCTAVE_MAX;

  // Reset all octave LEDs to OFF first (false = OFF for sink wiring)
  IOExpander_setLed(LED_OCT_1, false);
  IOExpander_setLed(LED_OCT_2, false);
  IOExpander_setLed(LED_OCT_3, false);
  IOExpander_setLed(LED_OCT_4, false);

  // Lower octaves (below center)
  if (octave < OCTAVE_CENTER) {

    // octave 1 → LED 2
    if (octave == 1) {
      IOExpander_setLed(LED_OCT_2, true);
    }

    // octave 0 → LED 1 + LED 2
    if (octave == 0) {
      IOExpander_setLed(LED_OCT_1, true);
    }
  }

  // Higher octaves (above center)
  if (octave > OCTAVE_CENTER) {

    // octave 3 → LED 3
    if (octave == 3) {
      IOExpander_setLed(LED_OCT_3, true);
    }

    // octave 4 → LED 3 + LED 4
    if (octave == 4) {
      IOExpander_setLed(LED_OCT_4, true);
    }
  }

  // octave == 2 → no LEDs on (center / neutral)

  // NOTE: do not call IOExpander_update() here — call IOExpander_update() from main loop()
  // to batch writes and avoid duplicated I2C traffic.
}
