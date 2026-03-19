
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
static constexpr byte LED_LFO_Mode_1 = PIN_LED_LFO_Mode_1;
static constexpr byte LED_LFO_Mode_2 = PIN_LED_LFO_Mode_2;
static constexpr byte LED_LFO_Mode_3 = PIN_LED_LFO_Mode_3;
static constexpr byte LED_LFO_Mode_4 = PIN_LED_LFO_Mode_4;
// ========================

/**
 * @brief Initialize LFO Mode LEDs
 * Call once in setup()
 */
void LED_LFO_Mode_begin() {
  // Nothing hardware-specific here.
  // Ensure IO Expander is initialized separately by calling IOExpander_begin() in setup().
}


/**
 * @brief Update LFO Mode LEDs according to LFOmodeSelect
 * Call regularly from loop()
 */
void LED_LFO_Mode_update() {

  // Reset all LFOmodeSelect LEDs to OFF first (false = OFF for sink wiring)
  IOExpander_setLed(LED_LFO_Mode_1, false);
  IOExpander_setLed(LED_LFO_Mode_2, false);
  IOExpander_setLed(LED_LFO_Mode_3, false);
  IOExpander_setLed(LED_LFO_Mode_4, false);

  // LFO Mode 1 → LED 1
  if (LFOmodeSelect == 0) {
    IOExpander_setLed(LED_LFO_Mode_1, true);
  }

  // LFO Mode 2 → LED 2
  if (LFOmodeSelect == 1) {
    IOExpander_setLed(LED_LFO_Mode_2, true);
  }
  
  // LFO Mode 3 → LED 3
  if (LFOmodeSelect == 2) {
    IOExpander_setLed(LED_LFO_Mode_3, true);
  }
  
  // LFO Mode 4 → LED 4
  if (LFOmodeSelect == 3) {
    IOExpander_setLed(LED_LFO_Mode_4, true);
  }
}