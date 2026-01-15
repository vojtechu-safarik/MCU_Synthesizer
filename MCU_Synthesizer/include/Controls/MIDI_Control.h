#ifndef MIDI_CONTROL_H
#define MIDI_CONTROL_H

#include <Arduino.h>

// MIDI CC control numbers (used in VirtualControlChange)
#define CCshape_1 53
#define CCshape_2 54
#define CCshape_3 55

#define CCoctave 59
#define CCLFOwaveSelect 81
// ===================

void VirtualControlChange(byte channel, byte control, byte value);
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);

#endif // MIDI_CONTROL_H
