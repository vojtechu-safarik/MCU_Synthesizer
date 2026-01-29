#ifndef HARDWARE_KEYBOARD_H
#define HARDWARE_KEYBOARD_H

#include <Arduino.h>

extern int sequenceBuffer[256];
extern byte sequenceLength;;

void Keyboard_init();

// update – zavoláš v loop()
void Keyboard_update();

void rebuildSequence();

// musí existovat jinde v projektu (už máš)
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);

#endif // HARDWARE_KEYBOARD_H