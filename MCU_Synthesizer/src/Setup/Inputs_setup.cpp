#include <Arduino.h>

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
#include "Configuration\Setup.h"
// ==================

// === CONTROLS ===
#include "Controls\Inputs.h"
// ==================

void Inputs_setup() {
// === setup POTENCIOMETRY + MUX (CD74HC4067)
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  // === setup PŘEPÍNAČE
  pinMode(SWsynthMode, INPUT_PULLUP);
  pinMode(SWLFOtype_1, INPUT_PULLUP);
  pinMode(SWLFOtype_3, INPUT_PULLUP);

  // === setup ENKODÉRU
  pinMode(ENC_CLK, INPUT);   // Set encoder pins as inputs
  pinMode(ENC_DT, INPUT);
  pinMode(ENC_SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(ENC_CLK);  // Read the initial state of CLK
}