
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

// --- direct MCU pins for LED 1 and 2 (active low) ---
static constexpr byte LED_Sequencer_Rate_1 = PIN_LED_Sequencer_Rate_1; // připojeno přímo na pin 14
static constexpr byte LED_Sequencer_Rate_2 = PIN_LED_Sequencer_Rate_2; // připojeno přímo na pin 38

// LED mapping on PCF pins
static constexpr byte LED_Sequencer_Rate_3 = PIN_LED_Sequencer_Rate_3;
static constexpr byte LED_Sequencer_Rate_4 = PIN_LED_Sequencer_Rate_4;
// ========================

/**
 * @brief Initialize Sequencer rate LEDs
 * Call once in setup()
 */
void LED_Sequencer_Rate_begin() {
  // Nothing hardware-specific here for expander (IOExpander_begin() zajištěno jinde).
  // Ensure IO Expander is initialized separately by calling IOExpander_begin() in setup().

  // Nastavíme přímo ovládané piny jako výstupy a do výchozího stavu (OFF, active-low -> HIGH).
  pinMode(LED_Sequencer_Rate_1, OUTPUT);
  pinMode(LED_Sequencer_Rate_2, OUTPUT);

  // Active-low: HIGH = vypnuto, LOW = zapnuto
  digitalWrite(LED_Sequencer_Rate_1, HIGH);
  digitalWrite(LED_Sequencer_Rate_2, HIGH);
}


/**
 * @brief Update Sequencer rate LEDs according to SeqRateSelect
 * Call regularly from loop()
 */
void LED_Sequencer_Rate_update() {
  
  // Reset all SeqRateSelect LEDs to OFF first
  // active-low => OFF = HIGH
  digitalWrite(LED_Sequencer_Rate_1, HIGH);
  digitalWrite(LED_Sequencer_Rate_2, HIGH);

  // false = OFF for sink wiring
  IOExpander_setLed(LED_Sequencer_Rate_3, false);
  IOExpander_setLed(LED_Sequencer_Rate_4, false);

  // Sequencer rate 1 → LED 1
  if (SeqRateSelect == 0) {
    digitalWrite(LED_Sequencer_Rate_1, LOW);
  }

  // Sequencer rate 2 → LED 2
  if (SeqRateSelect == 1) {
    digitalWrite(LED_Sequencer_Rate_2, LOW);
  }
  
  // Sequencer rate 3 → LED 3
  if (SeqRateSelect == 2) {
    IOExpander_setLed(LED_Sequencer_Rate_3, true);
  }
  
  // Sequencer rate 4 → LED 4
  if (SeqRateSelect == 3) {
    IOExpander_setLed(LED_Sequencer_Rate_4, true);
  }
}