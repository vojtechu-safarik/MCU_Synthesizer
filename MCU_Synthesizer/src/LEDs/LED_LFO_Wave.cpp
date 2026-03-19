
// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ========================

// === CONFIGURATION ===
#include "Configuration/Pins_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/IO_Expander.h"
#include "Hardware_Peripherals/LEDs.h"
// ==================

// LED mapping on PCF pins
static constexpr byte LED_LFO_Wave_1 = PIN_LED_LFO_Wave_1;
static constexpr byte LED_LFO_Wave_2 = PIN_LED_LFO_Wave_2;
static constexpr byte LED_LFO_Wave_3 = PIN_LED_LFO_Wave_3;
static constexpr byte LED_LFO_Wave_4 = PIN_LED_LFO_Wave_4;
static constexpr byte LED_LFO_Wave_5 = PIN_LED_LFO_Wave_5;
// ========================

/**
 * @brief Initialize LFO Wave LEDs
 * Call once in setup()
 */
void LED_LFO_Wave_begin() {
  // Nothing hardware-specific here.
  // Ensure IO Expander is initialized separately by calling IOExpander_begin() in setup().
}


/**
 * @brief Update LFO Wave LEDs according to LFOwaveSelect
 * Call regularly from loop()
 */
void LED_LFO_Wave_update() {

  // Reset all LFOwaveSelect LEDs to OFF first (false = OFF for sink wiring)
  IOExpander_setLed(LED_LFO_Wave_1, false);
  IOExpander_setLed(LED_LFO_Wave_2, false);
  IOExpander_setLed(LED_LFO_Wave_3, false);
  IOExpander_setLed(LED_LFO_Wave_4, false);
  IOExpander_setLed(LED_LFO_Wave_5, false);

  // LFO Wave 1 → LED 1
  if (LFOwaveSelect == 0) {
    IOExpander_setLed(LED_LFO_Wave_1, true);
  }

  // LFO Wave 2 → LED 2
  if (LFOwaveSelect == 1) {
    IOExpander_setLed(LED_LFO_Wave_2, true);
  }
  
  // LFO Wave 3 → LED 3
  if (LFOwaveSelect == 2) {
    IOExpander_setLed(LED_LFO_Wave_3, true);
  }
  
  // LFO Wave 4 → LED 4
  if (LFOwaveSelect == 3) {
    IOExpander_setLed(LED_LFO_Wave_4, true);
  }
   
  // LFO Wave 5 → LED 5
  if (LFOwaveSelect == 4) {
    IOExpander_setLed(LED_LFO_Wave_5, true);
  }
}