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

// ==============================
// ====== Project INCLUDES ======

// === WAVETABLES ===
#include "Wavetables\Saw_Wavetables_AA.h"        // contains saw1  ... saw11
#include "Wavetables\Square_Wavetables_AA.h"     // contains square1 ... square8
#include "Wavetables\Triangle_Wavetables_AA.h"   // contains triangle1  ... triangle8
#include "Wavetables\Sine_Wavetables_AA.h"       // contains sine1 ... sineXY
#include "Wavetables\Instrument_Wavetables_AA.h" // contains instrument1  ... instrumentXY
#include "Wavetables\Voice_Wavetables_AA.h"      // contains voice1 ... voiceXY
#include "Wavetables\LFO_Wavetables.h"           // contains sine, sawRISE, sawFALL, square, noise
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
#include "Global_Variables\Wavetable_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\BPMLock.h"
#include "Synthesis\Drone.h"
#include "Synthesis\Filter_Envelope.h"
#include "Synthesis\updateMorphWaveform.h"
#include "Synthesis\LFO.h"
#include "Synthesis\Portamento.h"
#include "Synthesis\ShapeDetuneRecomputeFix.h"
#include "Synthesis\BPMLock.h"
#include "Synthesis\KBD_Tracking.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\resetAllVoices.h"
#include "Voices\oscPlayVoice.h"
#include "Voices\oscStopVoice.h"
#include "Voices\updateVoices.h"
// ==================

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
#include "Configuration\Setup.h"
// ==================

// === CONTROLS ===
#include "Display\Display.h"
// ==================

// === CONTROLS ===
#include "Controls\MIDI_Control.h"
#include "Controls\Inputs.h"
// ==================

// ====== end Project INCLUDES ======
// ==================================

// ===================

// ============== Setup ==============
void setup() {
  Serial.begin(115200);
  AudioMemory(64);

  // Pure Data MIDI control
  usbMIDI.setHandleControlChange(VirtualControlChange);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleNoteOn(myNoteOn);

  Teensy_Audio_setup();

  recomputeKBDmult();

  Display_setup();

  Inputs_setup();

// ============================================
}

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
    
    Serial.print("Voice 1 Active: ");
    Serial.println(voiceActive_1);
    Serial.print("Voice 2 Active: ");
    Serial.println(voiceActive_2);
    Serial.print("Voice 3 Active: ");
    Serial.println(voiceActive_3);
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
        LFOupdate();
        DroneLFO_update();
        lfoUpdateTimer = 0;
    }
  updateEnvelopesPerVoice();
  updateVoices();  

  // Zpracovat pending save požadavky ze vstupu (PD/MIDI)
  for (uint8_t s = 0; s < 8; ++s) {
    if (DRONE_requestSave[s]) {
      // volání, které zkopíruje potřebné globální proměnné do snapshotu
      DroneSaveValues(s);
      DRONE_requestSave[s] = false;
    }
  }

  checkMux();

  Display_loop();
}

