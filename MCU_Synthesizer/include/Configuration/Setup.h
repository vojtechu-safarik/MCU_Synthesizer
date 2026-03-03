#ifndef SETUP_H
#define SETUP_H

// Function declarations
void Teensy_Audio_setup();     // Teensy Audio System Designer setup 
void Encoder_and_Mux_setup();  // Encoder and Multiplexors (pins) setup

// all setup functions at once (run once in main loop)
void Synthesizer_setup(); 

#endif // SETUP_H