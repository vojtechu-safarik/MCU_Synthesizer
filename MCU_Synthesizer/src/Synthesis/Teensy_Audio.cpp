#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
// ==================

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform_1;                 //xy=130,200
AudioSynthWaveform       waveform_2;                 //xy=130,275
AudioSynthWaveform       waveform_3;                 //xy=130,350
AudioSynthWaveform       waveformSub;                //xy=130,500
AudioSynthWaveform       waveformClone_1;            //xy=130,575
AudioSynthWaveform       waveformClone_2;            //xy=130,650
AudioSynthWaveform       waveformClone_3;            //xy=130,725
AudioSynthWaveform       waveformDroneA_1;           //xy=130,875
AudioSynthWaveform       waveformDroneA_2;           //xy=130,950
AudioSynthWaveform       waveformDroneA_3;           //xy=130,1025
AudioSynthWaveform       waveformDroneA_4;           //xy=130,1100
AudioSynthWaveform       waveformDroneA_5;           //xy=130,1175
AudioSynthWaveform       waveformDroneB_1;           //xy=130,1275
AudioSynthWaveform       waveformDroneB_2;           //xy=130,1350
AudioSynthWaveform       waveformDroneB_3;           //xy=130,1425
AudioSynthWaveform       waveformDroneB_4;           //xy=130,1500
AudioSynthWaveform       waveformDroneB_5;           //xy=130,1575
AudioSynthNoisePink      pink_1;                     //xy=545,1725
AudioSynthNoisePink      pinkDroneA;                 //xy=545,1800
AudioSynthNoisePink      pinkDroneB;                 //xy=545,1875
AudioAmplifier           amp_1;                      //xy=280,200
AudioAmplifier           amp_2;                      //xy=280,275
AudioAmplifier           amp_3;                      //xy=280,350
AudioAmplifier           ampSub;                     //xy=280,500
AudioAmplifier           ampClone_1;                 //xy=325,575
AudioAmplifier           ampClone_2;                 //xy=325,650
AudioAmplifier           ampClone_3;                 //xy=325,725
AudioAmplifier           ampDroneA_1;                //xy=325,875
AudioAmplifier           ampDroneA_2;                //xy=325,950
AudioAmplifier           ampDroneA_3;                //xy=325,1025
AudioAmplifier           ampDroneA_4;                //xy=325,1100
AudioAmplifier           ampDroneA_5;                //xy=325,1175
AudioAmplifier           ampDroneB_1;                //xy=325,1275
AudioAmplifier           ampDroneB_2;                //xy=325,1350
AudioAmplifier           ampDroneB_3;                //xy=325,1425
AudioAmplifier           ampDroneB_4;                //xy=325,1500
AudioAmplifier           ampDroneB_5;                //xy=325,1575
AudioFilterLadder        ladder_1;                   //xy=640,200
AudioFilterLadder        ladder_2;                   //xy=640,275
AudioFilterLadder        ladder_3;                   //xy=640,350
AudioFilterLadder        ladderSub;                  //xy=640,500
AudioFilterLadder        ladderClone_1;              //xy=810,575
AudioFilterLadder        ladderClone_2;              //xy=810,650
AudioFilterLadder        ladderClone_3;              //xy=810,725
AudioFilterStateVariable SV_ladderDroneA_1;          //xy=790,875
AudioFilterStateVariable SV_ladderDroneA_2;          //xy=790,950
AudioFilterStateVariable SV_ladderDroneA_3;          //xy=790,1025
AudioFilterStateVariable SV_ladderDroneA_4;          //xy=790,1100
AudioFilterStateVariable SV_ladderDroneA_5;          //xy=790,1175
AudioFilterStateVariable SV_ladderDroneB_1;          //xy=790,1275
AudioFilterStateVariable SV_ladderDroneB_2;          //xy=790,1350
AudioFilterStateVariable SV_ladderDroneB_3;          //xy=790,1425
AudioFilterStateVariable SV_ladderDroneB_4;          //xy=790,1500
AudioFilterStateVariable SV_ladderDroneB_5;          //xy=790,1575
AudioFilterStateVariable filterHP_1;                 //xy=805,200
AudioFilterStateVariable filterHP_2;                 //xy=805,275
AudioFilterStateVariable filterHP_3;                 //xy=805,350
AudioFilterStateVariable filterSubHP;                //xy=850,500
AudioFilterStateVariable filterCloneHP_1;            //xy=1015,575
AudioFilterStateVariable filterCloneHP_2;            //xy=1015,650
AudioFilterStateVariable filterCloneHP_3;            //xy=1015,725
AudioFilterStateVariable filterDroneAHP_1;           //xy=1015,875
AudioFilterStateVariable filterDroneAHP_2;           //xy=1015,950
AudioFilterStateVariable filterDroneAHP_3;           //xy=1015,1025
AudioFilterStateVariable filterDroneAHP_4;           //xy=1015,1100
AudioFilterStateVariable filterDroneAHP_5;           //xy=1015,1175
AudioFilterStateVariable filterDroneBHP_1;           //xy=1015,1275
AudioFilterStateVariable filterDroneBHP_2;           //xy=1015,1350
AudioFilterStateVariable filterDroneBHP_3;           //xy=1015,1425
AudioFilterStateVariable filterDroneBHP_4;           //xy=1015,1500
AudioFilterStateVariable filterDroneBHP_5;           //xy=1015,1575
AudioFilterStateVariable filterNoiseLP_1;            //xy=765,1725
AudioFilterStateVariable filterNoiseDroneALP;        //xy=765,1800
AudioFilterStateVariable filterNoiseDroneBLP;        //xy=765,1875
AudioFilterStateVariable filterNoiseHP_1;            //xy=1000,1725
AudioFilterStateVariable filterNoiseDroneAHP;        //xy=1000,1800
AudioFilterStateVariable filterNoiseDroneBHP;        //xy=1000,1875
AudioEffectEnvelope      envelope_Amplitude_1;       //xy=1010,200
AudioEffectEnvelope      envelope_Amplitude_2;       //xy=1010,275
AudioEffectEnvelope      envelope_Amplitude_3;       //xy=1010,350
AudioEffectEnvelope      envelopeSub_Amplitude;      //xy=1100,500
AudioEffectEnvelope      envelopeClone_Amplitude_1;  //xy=1250,575
AudioEffectEnvelope      envelopeClone_Amplitude_2;  //xy=1250,650
AudioEffectEnvelope      envelopeClone_Amplitude_3;  //xy=1250,725
AudioEffectEnvelope      envelopeDroneA_Amplitude_1; //xy=1250,875
AudioEffectEnvelope      envelopeDroneA_Amplitude_2; //xy=1250,950
AudioEffectEnvelope      envelopeDroneA_Amplitude_3; //xy=1250,1025
AudioEffectEnvelope      envelopeDroneA_Amplitude_4; //xy=1250,1100
AudioEffectEnvelope      envelopeDroneA_Amplitude_5; //xy=1250,1175
AudioEffectEnvelope      envelopeDroneB_Amplitude_1; //xy=1250,1275
AudioEffectEnvelope      envelopeDroneB_Amplitude_2; //xy=1250,1350
AudioEffectEnvelope      envelopeDroneB_Amplitude_3; //xy=1250,1425
AudioEffectEnvelope      envelopeDroneB_Amplitude_4; //xy=1250,1500
AudioEffectEnvelope      envelopeDroneB_Amplitude_5; //xy=1250,1575
AudioEffectEnvelope      envelopeNoise_1;            //xy=1250,1725
AudioEffectEnvelope      envelopeNoiseDroneA;        //xy=1250,1800
AudioEffectEnvelope      envelopeNoiseDroneB;        //xy=1250,1875
AudioMixer4              mixer_1;                    //xy=1325,275
AudioMixer4              mixer_2;                    //xy=1550,650
AudioMixer4              mixer_3;                    //xy=1550,1025
AudioMixer4              mixer_4;                    //xy=1550,1425
AudioMixer4              mixer_5;                    //xy=1750,650
AudioMixer4              mixer_6;                    //xy=1900,1275
AudioMixer4              mixer_7;                    //xy=1550,1725
AudioAmplifier           amp_MASTER;                 //xy=2100,1275
AudioOutputI2S           i2s_1;                      //xy=2300,1275
AudioConnection          patchCord1(waveform_1, amp_1);
AudioConnection          patchCord2(waveform_2, amp_2);
AudioConnection          patchCord3(waveform_3, amp_3);
AudioConnection          patchCord4(waveformSub, ampSub);
AudioConnection          patchCord5(waveformClone_1, ampClone_1);
AudioConnection          patchCord6(waveformClone_2, ampClone_2);
AudioConnection          patchCord7(waveformClone_3, ampClone_3);
AudioConnection          patchCord8(waveformDroneA_1, ampDroneA_1);
AudioConnection          patchCord9(waveformDroneA_2, ampDroneA_2);
AudioConnection          patchCord10(waveformDroneA_3, ampDroneA_3);
AudioConnection          patchCord11(waveformDroneA_4, ampDroneA_4);
AudioConnection          patchCord12(waveformDroneA_5, ampDroneA_5);
AudioConnection          patchCord13(waveformDroneB_1, ampDroneB_1);
AudioConnection          patchCord14(waveformDroneB_2, ampDroneB_2);
AudioConnection          patchCord15(waveformDroneB_3, ampDroneB_3);
AudioConnection          patchCord16(waveformDroneB_4, ampDroneB_4);
AudioConnection          patchCord17(waveformDroneB_5, ampDroneB_5);
AudioConnection          patchCord18(amp_1, 0, ladder_1, 0);
AudioConnection          patchCord19(amp_2, 0, ladder_2, 0);
AudioConnection          patchCord20(amp_3, 0, ladder_3, 0);
AudioConnection          patchCord21(ampSub, 0, ladderSub, 0);
AudioConnection          patchCord22(ampClone_1, 0, ladderClone_1, 0);
AudioConnection          patchCord23(ampClone_2, 0, ladderClone_2, 0);
AudioConnection          patchCord24(ampClone_3, 0, ladderClone_3, 0);
AudioConnection          patchCord25(ampDroneA_1, 0, SV_ladderDroneA_1, 0);
AudioConnection          patchCord26(ampDroneA_2, 0, SV_ladderDroneA_2, 0);
AudioConnection          patchCord27(ampDroneA_3, 0, SV_ladderDroneA_3, 0);
AudioConnection          patchCord28(ampDroneA_4, 0, SV_ladderDroneA_4, 0);
AudioConnection          patchCord29(ampDroneA_5, 0, SV_ladderDroneA_5, 0);
AudioConnection          patchCord30(ampDroneB_1, 0, SV_ladderDroneB_1, 0);
AudioConnection          patchCord31(ampDroneB_2, 0, SV_ladderDroneB_2, 0);
AudioConnection          patchCord32(ampDroneB_3, 0, SV_ladderDroneB_3, 0);
AudioConnection          patchCord33(ampDroneB_4, 0, SV_ladderDroneB_4, 0);
AudioConnection          patchCord34(ampDroneB_5, 0, SV_ladderDroneB_5, 0);
AudioConnection          patchCord35(ladder_1, 0, filterHP_1, 0);
AudioConnection          patchCord36(ladder_2, 0, filterHP_2, 0);
AudioConnection          patchCord37(ladder_3, 0, filterHP_3, 0);
AudioConnection          patchCord38(ladderSub, 0, filterSubHP, 0);
AudioConnection          patchCord39(filterHP_1, 2, envelope_Amplitude_1, 0);
AudioConnection          patchCord40(filterHP_2, 2, envelope_Amplitude_2, 0);
AudioConnection          patchCord41(filterHP_3, 2, envelope_Amplitude_3, 0);
AudioConnection          patchCord42(ladderClone_1, 0, filterCloneHP_1, 0);
AudioConnection          patchCord43(ladderClone_2, 0, filterCloneHP_2, 0);
AudioConnection          patchCord44(ladderClone_3, 0, filterCloneHP_3, 0);
AudioConnection          patchCord45(SV_ladderDroneA_1, 0, filterDroneAHP_1, 0);
AudioConnection          patchCord46(SV_ladderDroneA_2, 0, filterDroneAHP_2, 0);
AudioConnection          patchCord47(SV_ladderDroneA_3, 0, filterDroneAHP_3, 0);
AudioConnection          patchCord48(SV_ladderDroneA_4, 0, filterDroneAHP_4, 0);
AudioConnection          patchCord49(SV_ladderDroneA_5, 0, filterDroneAHP_5, 0);
AudioConnection          patchCord50(SV_ladderDroneB_1, 0, filterDroneBHP_1, 0);
AudioConnection          patchCord51(SV_ladderDroneB_2, 0, filterDroneBHP_2, 0);
AudioConnection          patchCord52(SV_ladderDroneB_3, 0, filterDroneBHP_3, 0);
AudioConnection          patchCord53(SV_ladderDroneB_4, 0, filterDroneBHP_4, 0);
AudioConnection          patchCord54(SV_ladderDroneB_5, 0, filterDroneBHP_5, 0);
AudioConnection          patchCord55(filterSubHP, 2, envelopeSub_Amplitude, 0);
AudioConnection          patchCord56(envelope_Amplitude_1, 0, mixer_1, 0);
AudioConnection          patchCord57(envelope_Amplitude_2, 0, mixer_1, 1);
AudioConnection          patchCord58(envelope_Amplitude_3, 0, mixer_1, 2);
AudioConnection          patchCord59(filterCloneHP_1, 2, envelopeClone_Amplitude_1, 0);
AudioConnection          patchCord60(filterCloneHP_2, 2, envelopeClone_Amplitude_2, 0);
AudioConnection          patchCord61(filterCloneHP_3, 2, envelopeClone_Amplitude_3, 0);
AudioConnection          patchCord62(filterDroneAHP_1, 2, envelopeDroneA_Amplitude_1, 0);
AudioConnection          patchCord63(filterDroneAHP_2, 2, envelopeDroneA_Amplitude_2, 0);
AudioConnection          patchCord64(filterDroneAHP_3, 2, envelopeDroneA_Amplitude_3, 0);
AudioConnection          patchCord65(filterDroneAHP_4, 2, envelopeDroneA_Amplitude_4, 0);
AudioConnection          patchCord66(filterDroneAHP_5, 2, envelopeDroneA_Amplitude_5, 0);
AudioConnection          patchCord67(filterDroneBHP_1, 2, envelopeDroneB_Amplitude_1, 0);
AudioConnection          patchCord68(filterDroneBHP_2, 2, envelopeDroneB_Amplitude_2, 0);
AudioConnection          patchCord69(filterDroneBHP_3, 2, envelopeDroneB_Amplitude_3, 0);
AudioConnection          patchCord70(filterDroneBHP_4, 2, envelopeDroneB_Amplitude_4, 0);
AudioConnection          patchCord71(filterDroneBHP_5, 2, envelopeDroneB_Amplitude_5, 0);
AudioConnection          patchCord72(envelopeSub_Amplitude, 0, mixer_1, 3);
AudioConnection          patchCord73(envelopeClone_Amplitude_1, 0, mixer_2, 0);
AudioConnection          patchCord74(envelopeClone_Amplitude_2, 0, mixer_2, 1);
AudioConnection          patchCord75(envelopeClone_Amplitude_3, 0, mixer_2, 2);
AudioConnection          patchCord76(envelopeDroneA_Amplitude_1, 0, mixer_3, 0);
AudioConnection          patchCord77(envelopeDroneA_Amplitude_2, 0, mixer_3, 1);
AudioConnection          patchCord78(envelopeDroneA_Amplitude_3, 0, mixer_3, 2);
AudioConnection          patchCord79(envelopeDroneA_Amplitude_4, 0, mixer_3, 3);
AudioConnection          patchCord80(envelopeDroneA_Amplitude_5, 0, mixer_6, 1);
AudioConnection          patchCord81(envelopeDroneB_Amplitude_1, 0, mixer_4, 0);
AudioConnection          patchCord82(envelopeDroneB_Amplitude_2, 0, mixer_4, 1);
AudioConnection          patchCord83(envelopeDroneB_Amplitude_3, 0, mixer_4, 2);
AudioConnection          patchCord84(envelopeDroneB_Amplitude_4, 0, mixer_4, 3);
AudioConnection          patchCord85(envelopeDroneB_Amplitude_5, 0, mixer_6, 2);
AudioConnection          patchCord86(mixer_1, 0, mixer_5, 0);
AudioConnection          patchCord87(mixer_2, 0, mixer_5, 1);
AudioConnection          patchCord88(mixer_3, 0, mixer_5, 2);
AudioConnection          patchCord89(mixer_4, 0, mixer_5, 3);
AudioConnection          patchCord90(mixer_5, 0, mixer_6, 0);
AudioConnection          patchCord91(mixer_6, amp_MASTER);
AudioConnection          patchCord92(amp_MASTER, 0, i2s_1, 0);
AudioConnection          patchCord93(amp_MASTER, 0, i2s_1, 1);
AudioConnection          patchCord94(pink_1, 0, filterNoiseLP_1, 0);
AudioConnection          patchCord95(pinkDroneA, 0, filterNoiseDroneALP, 0);
AudioConnection          patchCord96(pinkDroneB, 0, filterNoiseDroneBLP, 0);
AudioConnection          patchCord97(filterNoiseLP_1, 0, filterNoiseHP_1, 0);
AudioConnection          patchCord98(filterNoiseDroneALP, 0, filterNoiseDroneAHP, 0);
AudioConnection          patchCord99(filterNoiseDroneBLP, 0, filterNoiseDroneBHP, 0);
AudioConnection          patchCord100(filterNoiseHP_1, 2, envelopeNoise_1, 0);
AudioConnection          patchCord101(filterNoiseDroneAHP, 2, envelopeNoiseDroneA, 0);
AudioConnection          patchCord102(filterNoiseDroneBHP, 2, envelopeNoiseDroneB, 0);
AudioConnection          patchCord103(envelopeNoise_1, 0, mixer_7, 0);
AudioConnection          patchCord104(envelopeNoiseDroneA, 0, mixer_7, 1);
AudioConnection          patchCord105(envelopeNoiseDroneB, 0, mixer_7, 2);
AudioConnection          patchCord106(mixer_7, 0, mixer_6, 3);
AudioControlSGTL5000     sgtl5000_1;                 //xy=2300,1025
// GUItool: end automatically generated code