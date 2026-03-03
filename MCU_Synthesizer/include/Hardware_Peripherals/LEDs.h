#ifndef LEDS_H
#define LEDS_H

// LED begin functions
void LEDoctave_begin();
void LED_Sequencer_Rate_begin();
void LED_LFO_Wave_begin();
void LED_Portamento_Rate_begin();
void LED_LFO_Mode_begin();

// LED update functions
void LEDoctave_update();
void LED_Sequencer_Rate_update();
void LED_LFO_Wave_update();
void LED_Portamento_Rate_update();
void LED_LFO_Mode_update();

void LEDs_begin();                  // all LED begin functions at once (run in main setup)
void LEDs_update();                 // all LED update functions at once (run in main loop)

#endif // LEDS_H