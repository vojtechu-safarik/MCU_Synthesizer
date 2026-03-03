#include <Arduino.h>

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ========================

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/IO_Expander.h"
#include "Hardware_Peripherals/LEDs.h"
// ==================

// --- direct MCU pins (active low) ---
static constexpr byte LED_Octave_1 = PIN_LED_Octave_1;
static constexpr byte LED_Octave_2 = PIN_LED_Octave_2;
static constexpr byte LED_Octave_3 = PIN_LED_Octave_3;
static constexpr byte LED_Octave_4 = PIN_LED_Octave_4;

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
  // Nastavíme přímo ovládané piny jako výstupy a do výchozího stavu (OFF, active-low -> HIGH).
  pinMode(LED_Octave_1, OUTPUT);
  pinMode(LED_Octave_2, OUTPUT);
  pinMode(LED_Octave_3, OUTPUT);
  pinMode(LED_Octave_4, OUTPUT);

  // Active-low: HIGH = vypnuto, LOW = zapnuto
  digitalWrite(LED_Octave_1, HIGH);
  digitalWrite(LED_Octave_2, HIGH);
  digitalWrite(LED_Octave_3, HIGH);
  digitalWrite(LED_Octave_4, HIGH);
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

  // Reset all octave LEDs to OFF first
  digitalWrite(LED_Octave_1, HIGH);
  digitalWrite(LED_Octave_2, HIGH);
  digitalWrite(LED_Octave_3, HIGH);
  digitalWrite(LED_Octave_4, HIGH);

  // Lower octaves (below center)
  if (octave < OCTAVE_CENTER) {

    // octave 1 → LED 2
    if (octave == 1) {
      digitalWrite(LED_Octave_2, LOW);
    }

    // octave 0 → LED 1 + LED 2
    if (octave == 0) {
      digitalWrite(LED_Octave_1, LOW);
    }
  }

  // Higher octaves (above center)
  if (octave > OCTAVE_CENTER) {

    // octave 3 → LED 3
    if (octave == 3) {
      digitalWrite(LED_Octave_3, LOW);
    }

    // octave 4 → LED 3 + LED 4
    if (octave == 4) {
      digitalWrite(LED_Octave_4, LOW);
    }
  }

  // octave == 2 → no LEDs on (center / neutral)
}
