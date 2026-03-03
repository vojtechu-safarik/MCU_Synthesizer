#include <Arduino.h>

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
#include "Configuration\Setup.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals\Encoder_and_Multiplexors.h"
// ==================

void Encoder_and_Mux_setup() {
// === Multiplexor setup (CD74HC4067)
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  // === Encoder setup (KY-040)
  pinMode(Encoder_CLK, INPUT);   // Set encoder pins as inputs
  pinMode(Encoder_DT, INPUT);
  pinMode(Encoder_SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(Encoder_CLK);  // Read the initial state of CLK
}