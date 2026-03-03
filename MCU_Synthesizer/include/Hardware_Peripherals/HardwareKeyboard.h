#ifndef HARDWARE_KEYBOARD_H
#define HARDWARE_KEYBOARD_H

#include <Arduino.h>

extern int sequenceBuffer[256];
extern byte sequenceLength;;

void Keyboard_init();

// Update regularly called in the main loop
void Keyboard_update();

// Rebuild sequencer buffer after a change
void rebuildSequence();

// MIDI function (defined in MIDI config)
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);

#endif // HARDWARE_KEYBOARD_H