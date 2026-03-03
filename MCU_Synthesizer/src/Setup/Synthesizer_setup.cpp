// === CONFIGURATION ===
#include "Configuration\Setup.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals\Display.h"
#include "Hardware_Peripherals/IO_Expander.h"
#include "Hardware_Peripherals/LEDs.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\KBD_Tracking.h"
// ==================

// all setup functions at once (run once in main loop)
void Synthesizer_setup() {

  // Teensy Audio System Designer setup 
  Teensy_Audio_setup();

  // Recompute keyboard tracking multiplier
  recomputeKBDmult();

  // init SPI communication and set up display variables
  Display_setup();

  // Encoder and Multiplexors (pins) setup
  Encoder_and_Mux_setup();

  // init I2C / PCF devices
  IOExpander_begin();
  
  // all LED begin functions at once
  LEDs_begin();
}