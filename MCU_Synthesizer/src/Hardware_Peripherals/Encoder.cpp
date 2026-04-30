// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/Pins_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/Display.h"
// ==================

// Encoder state variables
int currentStateCLK;
int lastStateCLK;
unsigned long lastEncButtonPress = 0;

void checkEncoder() {
  currentStateCLK = digitalRead(Encoder_CLK);  // Read the current state of CLK

  // if CLK state has changed
  if (currentStateCLK != lastStateCLK) {
    // decide the direction according to the DT state
    if (digitalRead(Encoder_DT) != currentStateCLK) {
      GlobalBPM++;  // anti-clockwise
    } else {
      GlobalBPM--;  // clockwise
    }

    // BPM value constrain
    GlobalBPM = constrain(GlobalBPM, 40, 240);
    headerDirty = true;
  }

  lastStateCLK = currentStateCLK;

  // Button (not used yet, implement Preset mode later)
  int btnState = digitalRead(Encoder_SW);
  if (btnState == LOW) {
    if (millis() - lastEncButtonPress > 50) {
      //Serial.println("Button pressed!");
    }
    lastEncButtonPress = millis();
  }
  delay(1);
}  