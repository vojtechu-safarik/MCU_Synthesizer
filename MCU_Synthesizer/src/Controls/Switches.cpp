// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
// ==================

// === DISPLAY FUNCTIONS ===
#include "Display\Display.h"
// ==================

void checkSwitch() {

  // SynthMode
  if (digitalRead(SWsynthMode)) {
    SynthMode = 1;
  } else {
    SynthMode = 0;
  }
  
  if (prevSynthMode != SynthMode) {
    headerDirty = true;
  }

  // LFOtypeSelect
  if (digitalRead(SWLFOtype_1) && !digitalRead(SWLFOtype_3)) {
  LFOtypeSelect = 0;   // levá poloha
  }
  else if (!digitalRead(SWLFOtype_1) && digitalRead(SWLFOtype_3)) {
    LFOtypeSelect = 2;   // pravá poloha
  }
  else {
    LFOtypeSelect = 1;   // střed / žádná krajní (fallback)
  }
  
  /*
  static unsigned long lastPrintTime = 0;
  unsigned long nowCPUtest = millis();
  if (nowCPUtest - lastPrintTime >= 1000) {
    lastPrintTime = nowCPUtest;
    Serial.print("Current Shape Index 1: ");
    Serial.println(currentShapeIndex_1);
    Serial.print("Current Shape Index 2: ");
    Serial.println(currentShapeIndex_2);
    Serial.print("Current Shape Index 3: ");
    Serial.println(currentShapeIndex_3);
    Serial.print("Current LFO type: ");
    Serial.println(LFOtypeSelect);
  }
  */
}