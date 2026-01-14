// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
// ==================

// === CONTROLS ===
#include "Controls\MIDI_Control.h"
#include "Controls\Inputs.h"
// ==================

void checkMux() {
  static byte muxInput = 0;
  static int muxValues[MUX_control] = {};  // uchovává poslední hodnoty
  unsigned long currentMicros = micros();
  static unsigned long lastTime = 0;

  // spouštěj každých 1000 µs (1 ms)
  if (currentMicros - lastTime >= 1000) {
    lastTime = currentMicros;

    // přečti aktuální kanál
    int muxRead = analogRead(MUX_PIN);

    // jednoduchý deadband ±7 pro filtraci šumu
    if (muxRead > (muxValues[muxInput] + 7) || 
        muxRead < (muxValues[muxInput] - 7)) {
      
      muxValues[muxInput] = muxRead;
      muxRead = (muxRead >> 3); //Change range to 0-127
      
      switch (muxInput) {
        case MUXshape_1:
            VirtualControlChange(0, CCshape_1, muxRead);
            break;
        case MUXshape_2:
            VirtualControlChange(0, CCshape_2, muxRead);
            break;
        case MUXshape_3:
            VirtualControlChange(0, CCshape_3, muxRead);
            break;
      }
    }  

    // přepni na další MUX kanál
    muxInput++;
    if (muxInput >= MUX_control) muxInput = 0;

    // nastav adresové piny MUXu
    digitalWrite(MUX_S0, muxInput & B0001);
    digitalWrite(MUX_S1, muxInput & B0010);
    digitalWrite(MUX_S2, muxInput & B0100);
    digitalWrite(MUX_S3, muxInput & B1000);

    checkSwitch();
    checkEncoder();
  }
}