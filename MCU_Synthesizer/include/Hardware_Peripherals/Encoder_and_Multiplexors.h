#ifndef ENCODER_AND_MULTIPLEXORS_H
#define ENCODER_AND_MULTIPLEXORS_H

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// Encoder state variables
extern int currentStateCLK;
extern int lastStateCLK;
extern unsigned long lastEncButtonPress;

// Function declarations
void checkMux();                                         // Scan multiplexer inputs
void checkEncoder();                                     // Read rotary encoder

#endif // ENCODER_AND_MULTIPLEXORS_H