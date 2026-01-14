// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
// ==================

// === DISPLAY FUNCTIONS ===
#include "Display\Display.h"
// ==================

// Encoder state variables
int currentStateCLK;
int lastStateCLK;
unsigned long lastEncButtonPress = 0;

void checkEncoder() {
  currentStateCLK = digitalRead(ENC_CLK);  // Read the current state of CLK

  // Pokud se stav CLK změnil
  if (currentStateCLK != lastStateCLK) {
    // Urči směr podle stavu DT
    if (digitalRead(ENC_DT) != currentStateCLK) {
      GlobalBPM--;  // proti směru
    } else {
      GlobalBPM++;  // po směru
    }

    // omez hodnotu BPM
    GlobalBPM = constrain(GlobalBPM, 40, 240);
    headerDirty = true;
    
    /* // debug 
    Serial.print("Global BPM: ");
    Serial.println(GlobalBPM);
    */
  }

  lastStateCLK = currentStateCLK;

  // --- Tlačítko (zatím nevyužito, ale necháme pro budoucí rozšíření) ---
  int btnState = digitalRead(ENC_SW);
  if (btnState == LOW) {
    if (millis() - lastEncButtonPress > 50) {
      Serial.println("Button pressed!");
    }
    lastEncButtonPress = millis();
  }
  delay(1);
}  