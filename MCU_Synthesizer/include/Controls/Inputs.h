#ifndef INPUTS_H
#define INPUTS_H

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// Encoder state variables
extern int currentStateCLK;
extern int lastStateCLK;
extern unsigned long lastEncButtonPress;

// FUNCTION DECLARATIONS
void checkMux();                                         // Scan multiplexer inputs
void checkSwitch();                                      // Read switch states
void checkEncoder();                                     // Read rotary encoder

#endif // INPUTS_H