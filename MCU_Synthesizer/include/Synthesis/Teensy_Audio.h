#ifndef TEENSY_AUDIO_H
#define TEENSY_AUDIO_H

#include <Audio.h>

// WAVEFORMS
extern AudioSynthWaveform waveform_1;
extern AudioSynthWaveform waveform_2;
extern AudioSynthWaveform waveform_3;
extern AudioSynthWaveform waveformSub;

extern AudioSynthWaveform waveformClone_1;
extern AudioSynthWaveform waveformClone_2;
extern AudioSynthWaveform waveformClone_3;

extern AudioSynthWaveform waveformDroneA_1;
extern AudioSynthWaveform waveformDroneA_2;
extern AudioSynthWaveform waveformDroneA_3;
extern AudioSynthWaveform waveformDroneA_4;
extern AudioSynthWaveform waveformDroneA_5;

extern AudioSynthWaveform waveformDroneB_1;
extern AudioSynthWaveform waveformDroneB_2;
extern AudioSynthWaveform waveformDroneB_3;
extern AudioSynthWaveform waveformDroneB_4;
extern AudioSynthWaveform waveformDroneB_5;

// NOISE
extern AudioSynthNoisePink pink_1;
extern AudioSynthNoisePink pinkDroneA;
extern AudioSynthNoisePink pinkDroneB;

// AMPLIFIERS
extern AudioAmplifier amp_1;
extern AudioAmplifier amp_2;
extern AudioAmplifier amp_3;
extern AudioAmplifier ampSub;

extern AudioAmplifier ampClone_1;
extern AudioAmplifier ampClone_2;
extern AudioAmplifier ampClone_3;

extern AudioAmplifier ampDroneA_1;
extern AudioAmplifier ampDroneA_2;
extern AudioAmplifier ampDroneA_3;
extern AudioAmplifier ampDroneA_4;
extern AudioAmplifier ampDroneA_5;

extern AudioAmplifier ampDroneB_1;
extern AudioAmplifier ampDroneB_2;
extern AudioAmplifier ampDroneB_3;
extern AudioAmplifier ampDroneB_4;
extern AudioAmplifier ampDroneB_5;

extern AudioAmplifier amp_MASTER;

// FILTERS
extern AudioFilterLadder ladder_1;
extern AudioFilterLadder ladder_2;
extern AudioFilterLadder ladder_3;
extern AudioFilterLadder ladderSub;

extern AudioFilterLadder ladderClone_1;
extern AudioFilterLadder ladderClone_2;
extern AudioFilterLadder ladderClone_3;

extern AudioFilterStateVariable SV_ladderDroneA_1;
extern AudioFilterStateVariable SV_ladderDroneA_2;
extern AudioFilterStateVariable SV_ladderDroneA_3;
extern AudioFilterStateVariable SV_ladderDroneA_4;
extern AudioFilterStateVariable SV_ladderDroneA_5;

extern AudioFilterStateVariable SV_ladderDroneB_1;
extern AudioFilterStateVariable SV_ladderDroneB_2;
extern AudioFilterStateVariable SV_ladderDroneB_3;
extern AudioFilterStateVariable SV_ladderDroneB_4;
extern AudioFilterStateVariable SV_ladderDroneB_5;

extern AudioFilterStateVariable filterHP_1;
extern AudioFilterStateVariable filterHP_2;
extern AudioFilterStateVariable filterHP_3;
extern AudioFilterStateVariable filterSubHP;

extern AudioFilterStateVariable filterCloneHP_1;
extern AudioFilterStateVariable filterCloneHP_2;
extern AudioFilterStateVariable filterCloneHP_3;

extern AudioFilterStateVariable filterDroneAHP_1;
extern AudioFilterStateVariable filterDroneAHP_2;
extern AudioFilterStateVariable filterDroneAHP_3;
extern AudioFilterStateVariable filterDroneAHP_4;
extern AudioFilterStateVariable filterDroneAHP_5;

extern AudioFilterStateVariable filterDroneBHP_1;
extern AudioFilterStateVariable filterDroneBHP_2;
extern AudioFilterStateVariable filterDroneBHP_3;
extern AudioFilterStateVariable filterDroneBHP_4;
extern AudioFilterStateVariable filterDroneBHP_5;

extern AudioFilterStateVariable filterNoiseLP_1;
extern AudioFilterStateVariable filterNoiseHP_1;

extern AudioFilterStateVariable filterNoiseDroneALP;
extern AudioFilterStateVariable filterNoiseDroneAHP;

extern AudioFilterStateVariable filterNoiseDroneBLP;
extern AudioFilterStateVariable filterNoiseDroneBHP;

// ENVELOPES
extern AudioEffectEnvelope envelope_Amplitude_1;
extern AudioEffectEnvelope envelope_Amplitude_2;
extern AudioEffectEnvelope envelope_Amplitude_3;
extern AudioEffectEnvelope envelopeSub_Amplitude;

extern AudioEffectEnvelope envelopeClone_Amplitude_1;
extern AudioEffectEnvelope envelopeClone_Amplitude_2;
extern AudioEffectEnvelope envelopeClone_Amplitude_3;

extern AudioEffectEnvelope envelopeDroneA_Amplitude_1;
extern AudioEffectEnvelope envelopeDroneA_Amplitude_2;
extern AudioEffectEnvelope envelopeDroneA_Amplitude_3;
extern AudioEffectEnvelope envelopeDroneA_Amplitude_4;
extern AudioEffectEnvelope envelopeDroneA_Amplitude_5;

extern AudioEffectEnvelope envelopeDroneB_Amplitude_1;
extern AudioEffectEnvelope envelopeDroneB_Amplitude_2;
extern AudioEffectEnvelope envelopeDroneB_Amplitude_3;
extern AudioEffectEnvelope envelopeDroneB_Amplitude_4;
extern AudioEffectEnvelope envelopeDroneB_Amplitude_5;

extern AudioEffectEnvelope envelopeNoise_1;
extern AudioEffectEnvelope envelopeNoiseDroneA;
extern AudioEffectEnvelope envelopeNoiseDroneB;

// MIXERS / OUTPUT
extern AudioMixer4 mixer_1;
extern AudioMixer4 mixer_2;
extern AudioMixer4 mixer_3;
extern AudioMixer4 mixer_4;
extern AudioMixer4 mixer_5;
extern AudioMixer4 mixer_6;
extern AudioMixer4 mixer_7;

extern AudioOutputI2S i2s_1;
extern AudioControlSGTL5000 sgtl5000_1;

#endif // TEENSY_AUDIO_H
