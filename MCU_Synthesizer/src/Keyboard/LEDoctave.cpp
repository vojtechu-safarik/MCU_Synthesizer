#include <Arduino.h>

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ========================

// === LED CONFIGURATION ===

// LED pins (left → right)
static constexpr byte LED_OCT_1 = 33;
static constexpr byte LED_OCT_2 = 34;
static constexpr byte LED_OCT_3 = 35;
static constexpr byte LED_OCT_4 = 36;

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
  pinMode(LED_OCT_1, OUTPUT);
  pinMode(LED_OCT_2, OUTPUT);
  pinMode(LED_OCT_3, OUTPUT);
  pinMode(LED_OCT_4, OUTPUT);

  // Ensure all LEDs are off at startup
  digitalWrite(LED_OCT_1, LOW);
  digitalWrite(LED_OCT_2, LOW);
  digitalWrite(LED_OCT_3, LOW);
  digitalWrite(LED_OCT_4, LOW);
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

  // Reset all LEDs
  digitalWrite(LED_OCT_1, LOW);
  digitalWrite(LED_OCT_2, LOW);
  digitalWrite(LED_OCT_3, LOW);
  digitalWrite(LED_OCT_4, LOW);

  // Lower octaves (below center)
  if (octave < OCTAVE_CENTER) {

    // octave 1 → LED 2
    if (octave <= 1) {
      digitalWrite(LED_OCT_2, HIGH);
    }

    // octave 0 → LED 1 + LED 2
    if (octave == 0) {
      digitalWrite(LED_OCT_1, HIGH);
    }
  }

  // Higher octaves (above center)
  if (octave > OCTAVE_CENTER) {

    // octave 3 → LED 3
    if (octave >= 3) {
      digitalWrite(LED_OCT_3, HIGH);
    }

    // octave 4 → LED 3 + LED 4
    if (octave == 4) {
      digitalWrite(LED_OCT_4, HIGH);
    }
  }

  // octave == 2 → no LEDs on (center / neutral)
}
