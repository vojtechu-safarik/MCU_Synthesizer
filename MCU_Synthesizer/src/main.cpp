
// =======================
// ====== LIBRARIES ======

#include <Arduino.h>

#include "Audio.h"
#include "Wire.h"
#include "SPI.h"
#include "SD.h"
#include "SerialFlash.h"
#include "ILI9341_t3n.h"
#include "ili9341_t3n_OCR_A_Custom_Font.h"

#include "core_pins.h"
#include "imxrt.h"

#include "ResponsiveAnalogRead.h"

// === KEYPAD lib ===
#include "Key.h"
#include "Keypad.h"
// ==================

// ====== end LIBRARIES ======
// ===========================

// ==============================
// ====== Project INCLUDES ======

// === WAVETABLES ===
#include "Wavetables/Saw_Wavetables_AA.h"        // contains saw1  ... saw11
#include "Wavetables/Square_Wavetables_AA.h"     // contains square1 ... square8
#include "Wavetables/Triangle_Wavetables_AA.h"   // contains triangle1  ... triangle8
#include "Wavetables/Sine_Wavetables_AA.h"       // contains sine1 ... sineXY
#include "Wavetables/Instrument_Wavetables_AA.h" // contains instrument1  ... instrumentXY
#include "Wavetables/Voice_Wavetables_AA.h"      // contains voice1 ... voiceXY
#include "Wavetables/LFO_Wavetables.h"           // contains sine, sawRISE, sawFALL, square, noise
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
#include "Global_Variables/Wavetable_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis/Teensy_Audio.h"
#include "Synthesis/Drone.h"
#include "Synthesis/Filter_Envelope.h"
#include "Synthesis/updateMorphWaveform.h"
#include "Synthesis/LFO.h"
#include "Synthesis/Portamento.h"
#include "Synthesis/KBD_Tracking.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices/resetAllVoices.h"
#include "Voices/oscPlayVoice.h"
#include "Voices/oscStopVoice.h"
#include "Voices/updateVoices.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/Pins_Config.h"
#include "Configuration/Setup.h"
#include "Configuration/MIDI_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/Display.h"
#include "Hardware_Peripherals/IO_Expander.h"
#include "Hardware_Peripherals/Encoder_and_Multiplexors.h"
#include "Hardware_Peripherals/LEDs.h"
#include "Hardware_Peripherals/HardwareKeyboard.h"
// ==================

// ====== end Project INCLUDES ======
// ==================================

// ============== Setup ==============
void setup() {
  Serial.begin(115200);
  AudioMemory(64);

  // USB MIDI configuration
  usbMIDI.setHandleControlChange(VirtualControlChange);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);

  // all setup functions at once (run once in main loop)
  Synthesizer_setup(); 
} // ================================

// ============== Loop ==============
void loop() {
  /*
  // CPU and memory usage 
  static unsigned long lastPrintTime = 0;
  unsigned long nowCPUtest = millis();
  if (nowCPUtest - lastPrintTime >= 1000) { // 1 second
    lastPrintTime = nowCPUtest;
    
    Serial.print("Audio Memory Usage: ");
    Serial.println(AudioMemoryUsageMax());
    Serial.print("Filter CPU Usage: ");
    Serial.print(ladder_1.processorUsageMax());
    Serial.print("%, Total CPU Usage: ");
    Serial.print(AudioProcessorUsageMax());
    Serial.println("%");
  }
  */

  /*
  static unsigned long lastPrintTime = 0;
  unsigned long nowShapeIndextest = millis();
  if (nowShapeIndextest - lastPrintTime >= 1000) { // 1 second
    lastPrintTime = nowShapeIndextest;
    Serial.print("Current shapeIndex_1 = "); Serial.println(currentShapeIndex_1);
    Serial.print("Current shapeIndex_2 = "); Serial.println(currentShapeIndex_2);
    Serial.print("Current shapeIndex_3 = "); Serial.println(currentShapeIndex_3);
  }
  */

  // Read the values for virtual MIDI control at the start of the loop
  usbMIDI.read();
    static elapsedMillis lfoUpdateTimer;
    if (lfoUpdateTimer >= LFO_UPDATE_INTERVAL) {
        LFO_update();
        DroneLFO_update();
        lfoUpdateTimer = 0;
    }
  updateEnvelopesPerVoice();
  updateVoices(); 
  
  if ((CurrentSeqMode != 0) && (LFOtypeSelect == 2)) {
    LFO_update();
  }

  // process pending save requests from input
  for (uint8_t s = 0; s < 8; ++s) {
    if (DRONE_requestSave[s]) {
      // copy global variables into snapshot
      DroneSaveValues(s);
      DRONE_requestSave[s] = false;
    }
  }

  DroneSequencer_update();

  checkMux();

  Display_loop();

  // SMAZAT
  Keyboard_update();
  
  LEDs_update();        // all LED update functions at once - set flags for IOExpander_setLed()
  IOExpander_update();  // flush cached states to IO Expander - batch I2C write
  
}
