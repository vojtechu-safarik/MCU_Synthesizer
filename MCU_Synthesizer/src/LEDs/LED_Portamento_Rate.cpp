
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

// LED mapping on PCF pins
static constexpr byte LED_Portamento_Rate_1 = PIN_LED_Portamento_Rate_1;
static constexpr byte LED_Portamento_Rate_2 = PIN_LED_Portamento_Rate_2;
static constexpr byte LED_Portamento_Rate_3 = PIN_LED_Portamento_Rate_3;
static constexpr byte LED_Portamento_Rate_4 = PIN_LED_Portamento_Rate_4;
static constexpr byte LED_Portamento_Rate_5 = PIN_LED_Portamento_Rate_5;
// ========================

/**
 * @brief Initialize Portamento rate LEDs
 * Call once in setup()
 */
void LED_Portamento_Rate_begin() {
  // Nothing hardware-specific here.
  // Ensure IO Expander is initialized separately by calling IOExpander_begin() in setup().
}


/**
 * @brief Update Portamento rate LEDs according to PORTlockRate
 * Call regularly from loop()
 */
void LED_Portamento_Rate_update() {

  // Reset all PORTlockRate LEDs to OFF first (false = OFF for sink wiring)
  IOExpander_setLed(LED_Portamento_Rate_1, false);
  IOExpander_setLed(LED_Portamento_Rate_2, false);
  IOExpander_setLed(LED_Portamento_Rate_3, false);
  IOExpander_setLed(LED_Portamento_Rate_4, false);
  IOExpander_setLed(LED_Portamento_Rate_5, false);

  // Portamento rate 1 → LED 1
  if (PORTlockRate == 0) {
    IOExpander_setLed(LED_Portamento_Rate_1, true);
  }

  // Portamento rate 2 → LED 2
  if (PORTlockRate == 1) {
    IOExpander_setLed(LED_Portamento_Rate_2, true);
  }
  
  // Portamento rate 3 → LED 3
  if (PORTlockRate == 2) {
    IOExpander_setLed(LED_Portamento_Rate_3, true);
  }
  
  // Portamento rate 4 → LED 4
  if (PORTlockRate == 3) {
    IOExpander_setLed(LED_Portamento_Rate_4, true);
  }
   
  // Portamento rate 5 → LED 5
  if (PORTlockRate == 4) {
    IOExpander_setLed(LED_Portamento_Rate_5, true);
  }
}