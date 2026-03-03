
// === Hardware Peripherals ===
#include "Hardware_Peripherals/LEDs.h"
// ==================

// all LED begin functions at once (run in main setup)
void LEDs_begin() {
    LEDoctave_begin();
    LED_Sequencer_Rate_begin();
    LED_LFO_Wave_begin();
    LED_Portamento_Rate_begin();
    LED_LFO_Mode_begin();
}

// all LED update functions at once (run in main loop)
void LEDs_update() {
    LEDoctave_update();
    LED_Sequencer_Rate_update();
    LED_LFO_Wave_update();
    LED_Portamento_Rate_update();
    LED_LFO_Mode_update();    
}