#ifndef HARDWARE_KEYBOARD_H
#define HARDWARE_KEYBOARD_H

#include <Arduino.h>

// update – zavoláš v loop()
void Keyboard_update();

// musí existovat jinde v projektu (už máš)
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);

#endif // HARDWARE_KEYBOARD_H