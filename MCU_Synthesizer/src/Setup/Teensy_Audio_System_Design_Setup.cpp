// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\Drone.h"
#include "Synthesis\updateMorphWaveform.h"
// ==================

void Teensy_Audio_setup() {
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.85);    // max amplitude
  
  mixer_1.gain(0, 0.5);
  mixer_1.gain(1, 0.5);
  mixer_1.gain(2, 0.5);
  mixer_1.gain(3, 0); // sub-oscillator OFF - even if quiet, it changes waveform timbre
  mixer_2.gain(0, 0.5);
  mixer_2.gain(1, 0.5);
  mixer_2.gain(2, 0.5);
  mixer_3.gain(0, 0.5);
  mixer_3.gain(1, 0.5);
  mixer_3.gain(2, 0.5);
  mixer_3.gain(3, 0.5);
  mixer_4.gain(0, 0.5);
  mixer_4.gain(1, 0.5);
  mixer_4.gain(2, 0.5);
  mixer_4.gain(3, 0.5);
  mixer_5.gain(0, 0.5);
  mixer_5.gain(1, 0.5);
  mixer_5.gain(2, 0.5);
  mixer_5.gain(3, 0.5);
  mixer_6.gain(0, 0.5);
  mixer_6.gain(1, 0.5);
  mixer_6.gain(2, 0.5);

  // initialization of the first waveform
  updateMorphWaveform_1();
  waveform_1.begin(WAVEFORM_ARBITRARY);
  waveform_1.arbitraryWaveform(myWaveform_1, TABLE_SIZE);
  updateMorphWaveform_2();
  waveform_2.begin(WAVEFORM_ARBITRARY);
  waveform_2.arbitraryWaveform(myWaveform_2, TABLE_SIZE);
  updateMorphWaveform_3();
  waveform_3.begin(WAVEFORM_ARBITRARY);
  waveform_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  
  waveformSub.begin(WAVEFORM_ARBITRARY);
  waveformSub.arbitraryWaveform(myWaveform_1, TABLE_SIZE);

  if (SynthMode == 0) {
    waveformClone_1.begin(WAVEFORM_ARBITRARY);
    waveformClone_1.arbitraryWaveform(myWaveform_1, TABLE_SIZE);
    waveformClone_2.begin(WAVEFORM_ARBITRARY);
    waveformClone_2.arbitraryWaveform(myWaveform_2, TABLE_SIZE);
    waveformClone_3.begin(WAVEFORM_ARBITRARY);
    waveformClone_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  } else {
    waveformClone_1.begin(WAVEFORM_ARBITRARY);
    waveformClone_1.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
    waveformClone_2.begin(WAVEFORM_ARBITRARY);
    waveformClone_2.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
    waveformClone_3.begin(WAVEFORM_ARBITRARY);
    waveformClone_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  }
  // Drone A
  waveformDroneA_1.begin(WAVEFORM_ARBITRARY);
  waveformDroneA_1.arbitraryWaveform(myWaveform_1, TABLE_SIZE); 
  waveformDroneA_2.begin(WAVEFORM_ARBITRARY);
  waveformDroneA_2.arbitraryWaveform(myWaveform_2, TABLE_SIZE); 
  waveformDroneA_3.begin(WAVEFORM_ARBITRARY);
  waveformDroneA_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  // POLY drone A
  waveformDroneA_4.begin(WAVEFORM_ARBITRARY);
  waveformDroneA_4.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  waveformDroneA_5.begin(WAVEFORM_ARBITRARY);
  waveformDroneA_5.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  // Drone B
  waveformDroneB_1.begin(WAVEFORM_ARBITRARY);
  waveformDroneB_1.arbitraryWaveform(myWaveform_1, TABLE_SIZE); 
  waveformDroneB_2.begin(WAVEFORM_ARBITRARY);
  waveformDroneB_2.arbitraryWaveform(myWaveform_2, TABLE_SIZE); 
  waveformDroneB_3.begin(WAVEFORM_ARBITRARY);
  waveformDroneB_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  // POLY drone B
  waveformDroneB_4.begin(WAVEFORM_ARBITRARY);
  waveformDroneB_4.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
  waveformDroneB_5.begin(WAVEFORM_ARBITRARY);
  waveformDroneB_5.arbitraryWaveform(myWaveform_3, TABLE_SIZE);

  pink_1.amplitude(pinkVolume);
  pinkDroneA.amplitude(pinkVolume);
  pinkDroneB.amplitude(pinkVolume);

  // initial default values
  amp_1.gain(defaultVolume_1);
  amp_2.gain(defaultVolume_2);
  amp_3.gain(defaultVolume_3);
  ampSub.gain(defaultVolume_1);
  // clone amp settings inherited from unisonTriad
  if ((SynthMode == 0) && (unisonTriad == 1)) {
    ampClone_1.gain(defaultVolume_1);
    ampClone_2.gain(defaultVolume_2);
    ampClone_3.gain(defaultVolume_3);
  } else {
    ampClone_1.gain(defaultVolume_3);
    ampClone_2.gain(defaultVolume_3);
    ampClone_3.gain(defaultVolume_3);
  }
  amp_MASTER.gain(MASTERVolume);

  ampDroneA_1.gain(0);
  ampDroneA_2.gain(0);
  ampDroneA_3.gain(0);
  ampDroneA_4.gain(0);
  ampDroneA_5.gain(0);
  ampDroneB_1.gain(0);
  ampDroneB_2.gain(0);
  ampDroneB_3.gain(0);
  ampDroneB_4.gain(0);
  ampDroneB_5.gain(0);

  ladder_1.frequency(LPFcutoffFreq);
  ladder_1.resonance(LPFresonanceVal);
  ladder_2.frequency(LPFcutoffFreq);
  ladder_2.resonance(LPFresonanceVal);
  ladder_3.frequency(LPFcutoffFreq);
  ladder_3.resonance(LPFresonanceVal);
  ladderSub.frequency(LPFcutoffFreq);
  ladderSub.resonance(LPFresonanceVal);
  ladderClone_1.frequency(LPFcutoffFreq);
  ladderClone_1.resonance(LPFresonanceVal);
  ladderClone_2.frequency(LPFcutoffFreq);
  ladderClone_2.resonance(LPFresonanceVal);
  ladderClone_3.frequency(LPFcutoffFreq);
  ladderClone_3.resonance(LPFresonanceVal);
  SV_ladderDroneA_1.frequency(LPFcutoffFreq);
  SV_ladderDroneA_1.resonance(defaultSVLpfRes);
  SV_ladderDroneA_2.frequency(LPFcutoffFreq);
  SV_ladderDroneA_2.resonance(defaultSVLpfRes);
  SV_ladderDroneA_3.frequency(LPFcutoffFreq);
  SV_ladderDroneA_3.resonance(defaultSVLpfRes);
  SV_ladderDroneA_4.frequency(LPFcutoffFreq);
  SV_ladderDroneA_4.resonance(defaultSVLpfRes);
  SV_ladderDroneA_5.frequency(LPFcutoffFreq);
  SV_ladderDroneA_5.resonance(defaultSVLpfRes);  
  SV_ladderDroneB_1.frequency(LPFcutoffFreq);
  SV_ladderDroneB_1.resonance(defaultSVLpfRes);
  SV_ladderDroneB_2.frequency(LPFcutoffFreq);
  SV_ladderDroneB_2.resonance(defaultSVLpfRes);
  SV_ladderDroneB_3.frequency(LPFcutoffFreq);
  SV_ladderDroneB_3.resonance(defaultSVLpfRes);
  SV_ladderDroneB_4.frequency(LPFcutoffFreq);
  SV_ladderDroneB_4.resonance(defaultSVLpfRes);
  SV_ladderDroneB_5.frequency(LPFcutoffFreq);
  SV_ladderDroneB_5.resonance(defaultSVLpfRes);  

  filterNoiseLP_1.frequency(LPFcutoffFreq);
  filterNoiseLP_1.resonance(defaultSVLpfRes); 
  filterNoiseDroneALP.frequency(LPFcutoffFreq);
  filterNoiseDroneALP.resonance(defaultSVLpfRes); 
  filterNoiseDroneBLP.frequency(LPFcutoffFreq);
  filterNoiseDroneBLP.resonance(defaultSVLpfRes); 

  filterHP_1.frequency(defaultHpfFreq);
  filterHP_1.resonance(defaultHpfRes);
  filterHP_2.frequency(defaultHpfFreq);
  filterHP_2.resonance(defaultHpfRes);
  filterHP_3.frequency(defaultHpfFreq);
  filterHP_3.resonance(defaultHpfRes);
  filterSubHP.frequency(defaultHpfFreq);
  filterSubHP.resonance(defaultHpfRes);
  filterCloneHP_1.frequency(defaultHpfFreq);
  filterCloneHP_1.resonance(defaultHpfRes);
  filterCloneHP_2.frequency(defaultHpfFreq);
  filterCloneHP_2.resonance(defaultHpfRes);
  filterCloneHP_3.frequency(defaultHpfFreq);
  filterCloneHP_3.resonance(defaultHpfRes);
  filterDroneAHP_1.frequency(defaultHpfFreq);
  filterDroneAHP_1.resonance(defaultHpfRes);
  filterDroneAHP_2.frequency(defaultHpfFreq);
  filterDroneAHP_2.resonance(defaultHpfRes);
  filterDroneAHP_3.frequency(defaultHpfFreq);
  filterDroneAHP_3.resonance(defaultHpfRes);
  filterDroneAHP_4.frequency(defaultHpfFreq);
  filterDroneAHP_4.resonance(defaultHpfRes);
  filterDroneAHP_5.frequency(defaultHpfFreq);
  filterDroneAHP_5.resonance(defaultHpfRes);
  filterDroneBHP_1.frequency(defaultHpfFreq);
  filterDroneBHP_1.resonance(defaultHpfRes);
  filterDroneBHP_2.frequency(defaultHpfFreq);
  filterDroneBHP_2.resonance(defaultHpfRes);
  filterDroneBHP_3.frequency(defaultHpfFreq);
  filterDroneBHP_3.resonance(defaultHpfRes);
  filterDroneBHP_4.frequency(defaultHpfFreq);
  filterDroneBHP_4.resonance(defaultHpfRes);
  filterDroneBHP_5.frequency(defaultHpfFreq);
  filterDroneBHP_5.resonance(defaultHpfRes);

  filterNoiseHP_1.frequency(defaultHpfFreq);
  filterNoiseHP_1.resonance(defaultHpfRes); 
  filterNoiseDroneAHP.frequency(defaultHpfFreq);
  filterNoiseDroneAHP.resonance(defaultHpfRes); 
  filterNoiseDroneBHP.frequency(defaultHpfFreq);
  filterNoiseDroneBHP.resonance(defaultHpfRes); 

  envelope_Amplitude_1.attack(0);
  envelope_Amplitude_1.decay(0);
  envelope_Amplitude_1.sustain(1);
  envelope_Amplitude_1.release(500);
  envelope_Amplitude_2.attack(0);
  envelope_Amplitude_2.decay(0);
  envelope_Amplitude_2.sustain(1);
  envelope_Amplitude_2.release(500);
  envelope_Amplitude_3.attack(0);
  envelope_Amplitude_3.decay(0);
  envelope_Amplitude_3.sustain(1);
  envelope_Amplitude_3.release(500);
  envelopeSub_Amplitude.attack(0);
  envelopeSub_Amplitude.decay(0);
  envelopeSub_Amplitude.sustain(1);
  envelopeSub_Amplitude.release(500);
  envelopeClone_Amplitude_1.attack(0);
  envelopeClone_Amplitude_1.decay(0);
  envelopeClone_Amplitude_1.sustain(1);
  envelopeClone_Amplitude_1.release(500);
  envelopeClone_Amplitude_2.attack(0);
  envelopeClone_Amplitude_2.decay(0);
  envelopeClone_Amplitude_2.sustain(1);
  envelopeClone_Amplitude_2.release(500);
  envelopeClone_Amplitude_3.attack(0);
  envelopeClone_Amplitude_3.decay(0);
  envelopeClone_Amplitude_3.sustain(1);
  envelopeClone_Amplitude_3.release(500);
  envelopeDroneA_Amplitude_1.attack(0);
  envelopeDroneA_Amplitude_1.decay(0);
  envelopeDroneA_Amplitude_1.sustain(1);
  envelopeDroneA_Amplitude_1.release(500);
  envelopeDroneA_Amplitude_2.attack(0);
  envelopeDroneA_Amplitude_2.decay(0);
  envelopeDroneA_Amplitude_2.sustain(1);
  envelopeDroneA_Amplitude_2.release(500);
  envelopeDroneA_Amplitude_3.attack(0);
  envelopeDroneA_Amplitude_3.decay(0);
  envelopeDroneA_Amplitude_3.sustain(1);
  envelopeDroneA_Amplitude_3.release(500);
  envelopeDroneA_Amplitude_4.attack(0);
  envelopeDroneA_Amplitude_4.decay(0);
  envelopeDroneA_Amplitude_4.sustain(1);
  envelopeDroneA_Amplitude_4.release(500);
  envelopeDroneA_Amplitude_5.attack(0);
  envelopeDroneA_Amplitude_5.decay(0);
  envelopeDroneA_Amplitude_5.sustain(1);
  envelopeDroneA_Amplitude_5.release(500);
  envelopeDroneB_Amplitude_1.attack(0);
  envelopeDroneB_Amplitude_1.decay(0);
  envelopeDroneB_Amplitude_1.sustain(1);
  envelopeDroneB_Amplitude_1.release(500);
  envelopeDroneB_Amplitude_2.attack(0);
  envelopeDroneB_Amplitude_2.decay(0);
  envelopeDroneB_Amplitude_2.sustain(1);
  envelopeDroneB_Amplitude_2.release(500);
  envelopeDroneB_Amplitude_3.attack(0);
  envelopeDroneB_Amplitude_3.decay(0);
  envelopeDroneB_Amplitude_3.sustain(1);
  envelopeDroneB_Amplitude_3.release(500);
  envelopeDroneB_Amplitude_4.attack(0);
  envelopeDroneB_Amplitude_4.decay(0);
  envelopeDroneB_Amplitude_4.sustain(1);
  envelopeDroneB_Amplitude_4.release(500);
  envelopeDroneB_Amplitude_5.attack(0);
  envelopeDroneB_Amplitude_5.decay(0);
  envelopeDroneB_Amplitude_5.sustain(1);
  envelopeDroneB_Amplitude_5.release(500);

  envelopeNoise_1.attack(0);
  envelopeNoise_1.decay(0);
  envelopeNoise_1.sustain(1);
  envelopeNoise_1.release(500);
  envelopeNoiseDroneA.attack(0);
  envelopeNoiseDroneA.decay(0);
  envelopeNoiseDroneA.sustain(1);
  envelopeNoiseDroneA.release(500);
  envelopeNoiseDroneB.attack(0);
  envelopeNoiseDroneB.decay(0);
  envelopeNoiseDroneB.sustain(1);
  envelopeNoiseDroneB.release(500);
}