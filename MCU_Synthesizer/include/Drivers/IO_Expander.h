#ifndef IO_EXPANDER_H
#define IO_EXPANDER_H

#include <Arduino.h>

// Simple IO Expander interface used by modules (LEDoctave, others).
// - Call IOExpander_begin() once in setup()
// - Modules call IOExpander_setLed(logicalIndex, on/off)
// - Call IOExpander_update() in main loop regularly to flush changes to hardware

void IOExpander_begin();                  // initialize I2C / PCF devices
void IOExpander_update();                 // flush cached states to PCF (call from main loop)
void IOExpander_setLed(uint8_t idx, bool on); // set logical LED index (0..15) state (true=ON)
void IOExpander_setAllOff();              // convenience: set all known LEDs off (no immediate flush)

#endif // IO_EXPANDER_H