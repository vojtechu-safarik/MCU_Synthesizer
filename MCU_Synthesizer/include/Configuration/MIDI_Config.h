#ifndef MIDI_CONFIG_H
#define MIDI_CONFIG_H

#include <Arduino.h>

/* MIDI CC control numbers (used in VirtualControlChange) */
// Multiplexor 1
#define CCwave_1    47
#define CCwave_2    48
#define CCwave_3    49
#define CCshape_1   53
#define CCshape_2   54
#define CCshape_3   55
#define CCvolume_1  50
#define CCvolume_2  51
#define CCvolume_3  52
#define CCtune_1    56
#define CCtune_2    57
#define CCtune_3    58
#define CCnoise     87
#define CCsubOctave 60
// ======================

// Multiplexor 2
#define CC_LPF_Cutoff        67
#define CC_KBD_Track         68
#define CC_LPF_Res           69
#define CC_EG_Int            75
#define CC_HPF_Cutoff        70
#define CC_Filter_Attack     71
#define CC_Filter_Decay      72
#define CC_Filter_Sustain    73
#define CC_Filter_Release    74
#define CC_LFO_Rate          76
#define CC_LFO_Depth         77
#define CC_LFO_Delay         78
#define CC_Amplitude_Attack  63
#define CC_Amplitude_Decay   64
#define CC_Amplitude_Sustain 65
#define CC_Amplitude_Release 66
// ======================

// Multiplexor 3
#define CC_MasterVolume 85
#define CCseqGatePot 88
// ======================

#define CCoctave 59
#define CCLFOwaveSelect 81

// ===================

void VirtualControlChange(byte channel, byte control, byte value);
void myNoteOn(byte channel, byte note, byte velocity);
void myNoteOff(byte channel, byte note, byte velocity);

#endif // MIDI_CONFIG_H
