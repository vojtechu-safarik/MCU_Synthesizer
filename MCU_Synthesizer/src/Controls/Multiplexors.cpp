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
  static int MUX_1_values[MUX_1_control] = {};  // holds the last value of MUX_1
  static int MUX_2_values[MUX_2_control] = {};  // holds the last value of MUX_2
  unsigned long currentMicros = micros();
  static unsigned long lastTime = 0;

  // Three-way toggle switches (MUX only reads one channel at once)
  static bool LFOtype_1 = false;
  static bool LFOtype_3 = false;

  // spouštěj každých 1000 µs (1 ms)
  if (currentMicros - lastTime >= 1000) {
    lastTime = currentMicros;

    // nastav adresové piny MUXu
    digitalWrite(MUX_S0, muxInput & B0001);
    digitalWrite(MUX_S1, muxInput & B0010);
    digitalWrite(MUX_S2, muxInput & B0100);
    digitalWrite(MUX_S3, muxInput & B1000);

    // === MUX #1 ===
    // if MUX #1 exists, read the value
    if (muxInput < MUX_1_control) {
      int MUX_1_read = analogRead(MUX_1_PIN);

      // jednoduchý deadband ±7 pro filtraci šumu
      if (MUX_1_read > (MUX_1_values[muxInput] + 7) || 
          MUX_1_read < (MUX_1_values[muxInput] - 7)) {
        
        MUX_1_values[muxInput] = MUX_1_read;
        MUX_1_read = (MUX_1_read >> 3); //Change range to 0-127
        
        switch (muxInput) {
          case MUXshape_1:
              VirtualControlChange(0, CCshape_1, MUX_1_read);
              break;
          case MUXshape_2:
              VirtualControlChange(0, CCshape_2, MUX_1_read);
              break;
          case MUXshape_3:
              VirtualControlChange(0, CCshape_3, MUX_1_read);
              break;
          case MUXSeqGatePot:
              VirtualControlChange(0, CCseqGatePot, MUX_1_read);
              break;   
          case MUX_SynthMode:
              // pull-up: active LOW
              SynthMode = (MUX_1_read < 64) ? 1 : 0;
              break;     
          case MUX_LFOtype_1:
              LFOtype_1 = (MUX_1_read < 64);
              break;
          case MUX_LFOtype_3:
              LFOtype_3 = (MUX_1_read < 64);
              break;    
        }

        if (LFOtype_1 && !LFOtype_3) {
            LFOtypeSelect = 0;   // levá poloha
        } else if (!LFOtype_1 && LFOtype_3) {
            LFOtypeSelect = 2;   // pravá poloha
        } else {
            LFOtypeSelect = 1;   // střed / žádná krajní (fallback)
        }        
      }  
    }

    // === MUX #2 ===
    // if MUX #2 exists, read the value
    if (muxInput < MUX_2_control) {
      int MUX_2_read = analogRead(MUX_2_PIN);

      // jednoduchý deadband ±7 pro filtraci šumu
      if (MUX_2_read > (MUX_2_values[muxInput] + 7) || 
          MUX_2_read < (MUX_2_values[muxInput] - 7)) {
        
        MUX_2_values[muxInput] = MUX_2_read;
        MUX_2_read = (MUX_2_read >> 3); //Change range to 0-127
        
        switch (muxInput) {
          case MUX_LPF_Cutoff:
              VirtualControlChange(0, CC_LPF_Cutoff, MUX_2_read);
              break;
          case MUX_LPF_Res:
              VirtualControlChange(0, CC_LPF_Res, MUX_2_read);
              break;
          case MUX_KBD_Track:
              VirtualControlChange(0, CC_KBD_Track, MUX_2_read);
              break;
          case MUX_EG_Int:
              VirtualControlChange(0, CC_EG_Int, MUX_2_read);
              break;    
        }
      }  
    }

    // switch to next MUX channel --> "MUX_1_control = 16" as for 16 channels of CD74HC4067
    // --> its okay for all MUXes, even if one of them uses less channels
    muxInput++;
    if (muxInput >= MUX_1_control) muxInput = 0;

    checkSwitch();
    checkEncoder();  
  }
}
