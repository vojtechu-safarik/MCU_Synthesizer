// ==========================================
// ====== Headers required by function ======

// === WAVETABLES ===
#include "Wavetables\Saw_Wavetables_AA.h"        // contains saw1  ... saw11
#include "Wavetables\Square_Wavetables_AA.h"     // contains square1 ... square8
#include "Wavetables\Triangle_Wavetables_AA.h"   // contains triangle1  ... triangle8
#include "Wavetables\Sine_Wavetables_AA.h"       // contains sine1 ... sineXY
#include "Wavetables\Instrument_Wavetables_AA.h" // contains instrument1  ... instrumentXY
#include "Wavetables\Voice_Wavetables_AA.h"      // contains voice1 ... voiceXY
#include "Wavetables\LFO_Wavetables.h"           // contains sine, sawRISE, sawFALL, square, noise
// ==================

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\BPMLock.h"
#include "Synthesis\Drone.h"
#include "Synthesis\updateMorphWaveform.h"
#include "Synthesis\LFO.h"
#include "Synthesis\BPMLock.h"
#include "Synthesis\KBD_Tracking.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\resetAllVoices.h"
// ==================

// === CONTROLS ===
#include "Controls\MIDI_Control.h"
// ==================

// ====== end Headers required by function ======
// ==============================================

// ========= MIDI control =========
void VirtualControlChange(byte channel, byte control, byte value) {
  switch (control) {
    case 47: {
      if (value < NUM_WAVEFAMILIES) {
        // get old number of morph steps
        const WaveFamily& prevWF = waveFamilies[currentWaveFamily_1];
        int prevMaxIndex = (prevWF.numWaves - 1) * prevWF.morphSteps;
        float morphNorm = currentShapeIndex_1 / (float)prevMaxIndex;
        currentWaveFamily_1 = value;
        // recalculate the position according to new range
        const WaveFamily& newWF = waveFamilies[currentWaveFamily_1];
        int newMaxIndex = (newWF.numWaves - 1) * newWF.morphSteps;
        currentShapeIndex_1 = round(morphNorm * newMaxIndex);

        updateMorphWaveform_1();
      }
      break;
    }

    case 48: {
      if (value < NUM_WAVEFAMILIES) {
        const WaveFamily& prevWF = waveFamilies[currentWaveFamily_2];
        int prevMaxIndex = (prevWF.numWaves - 1) * prevWF.morphSteps;
        float morphNorm = currentShapeIndex_2 / (float)prevMaxIndex;
        currentWaveFamily_2 = value;
        const WaveFamily& newWF = waveFamilies[currentWaveFamily_2];
        int newMaxIndex = (newWF.numWaves - 1) * newWF.morphSteps;
        currentShapeIndex_2 = round(morphNorm * newMaxIndex);

        updateMorphWaveform_2();
      }
      break;
    }

    case 49: {
      if (value < NUM_WAVEFAMILIES) {
        const WaveFamily& prevWF = waveFamilies[currentWaveFamily_3];
        int prevMaxIndex = (prevWF.numWaves - 1) * prevWF.morphSteps;
        float morphNorm = currentShapeIndex_3 / (float)prevMaxIndex;
        currentWaveFamily_3 = value;
        const WaveFamily& newWF = waveFamilies[currentWaveFamily_3];
        int newMaxIndex = (newWF.numWaves - 1) * newWF.morphSteps;
        currentShapeIndex_3 = round(morphNorm * newMaxIndex);

        updateMorphWaveform_3();
      }
      break;
    }

    case 50: {
      // volume 0 .. 127 -> 0 .. 1 -> 0 .. 0.75
      defaultVolume_1 = value * DIV127 * 0.75f;

      if (SynthMode == 0) {
        amp_1.gain(defaultVolume_1);
        ampClone_1.gain(defaultVolume_1);
        ampSub.gain(defaultVolume_1);
      } 

      if (SynthMode == 1) {
        amp_1.gain(defaultVolume_1);
        ampSub.gain(defaultVolume_1);
      } 
      break;
    }

    case 51: {
      defaultVolume_2 = value * DIV127 * 0.75f;

      if (SynthMode == 0) {
        amp_2.gain(defaultVolume_2);
        ampClone_2.gain(defaultVolume_2);
      } 

      if (SynthMode == 1) {
        amp_2.gain(defaultVolume_2);
      } 
      break;
    }

    case 52: {
      defaultVolume_3 = value * DIV127 * 0.75f;

      if (SynthMode == 0) {
        amp_3.gain(defaultVolume_3);
        ampClone_3.gain(defaultVolume_3);
      }  

      if (SynthMode == 1) {
        amp_3.gain(defaultVolume_3);
        ampClone_1.gain(defaultVolume_3);
        ampClone_2.gain(defaultVolume_3);
        ampClone_3.gain(defaultVolume_3); 
      }
      break;
    }

    case CCshape_1: {
      // Morphing (slider in Pure Data)
      const WaveFamily& wf = waveFamilies[currentWaveFamily_1];
      int maxIndex = wf.morphSteps * (wf.numWaves - 1);
      currentShapeIndex_1 = map(value, 0, 127, 0, maxIndex);
      updateMorphWaveform_1();
      break;
    }

    case CCshape_2: {
      const WaveFamily& wf = waveFamilies[currentWaveFamily_2];
      int maxIndex = wf.morphSteps * (wf.numWaves - 1);
      currentShapeIndex_2 = map(value, 0, 127, 0, maxIndex);
      updateMorphWaveform_2();
      break;
    }
  
    case CCshape_3: {
      const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
      int maxIndex = wf.morphSteps * (wf.numWaves - 1);
      currentShapeIndex_3 = map(value, 0, 127, 0, maxIndex);
      updateMorphWaveform_3();
      break;
    }

    case 56: {
      Tune_1 = ((value * DIV127) * 24) - 12; // -12 .. 12 
      LFOTune_1 = Tune_1;
      Tune_1 = powf(2.0f, Tune_1 / 12.0f);

      if (SynthMode == 0) {
        LFOTune_4 = LFOTune_1;
        Tune_4 = Tune_1;
      } 
      break;
    }

    case 57: {
      Tune_2 = ((value * DIV127) * 24) - 12; // -12 .. 12 
      LFOTune_2 = Tune_2;
      Tune_2 = powf(2.0f, Tune_2 / 12.0f);
      
      if (SynthMode == 0) {
        LFOTune_5 = LFOTune_2;
        Tune_5 = Tune_2;
      } 
      break;
    }

    case 58: {
      Tune_3 = ((value * DIV127) * 24) - 12; // -12 .. 12 
      LFOTune_3 = Tune_3;
      Tune_3 = powf(2.0f, Tune_3 / 12.0f);

      if (SynthMode == 0) {
        LFOTune_5 = LFOTune_2;
        Tune_6 = Tune_3;
      } else if (SynthMode == 1) {
        LFOTune_4 = LFOTune_3;
        LFOTune_5 = LFOTune_3;
        LFOTune_6 = LFOTune_3;
        Tune_4 = Tune_3;
        Tune_5 = Tune_3;
        Tune_6 = Tune_3;
      }
      break;
    }    

    case CCoctave: {
      switch (value) {
        case 0:
          octave = 24;
          break;
        case 1:
          octave = 12;
          break;
        case 2:
          octave = 0;
          break;
        case 3:
          octave = -12;
          break;
        case 4:
          octave = -24;
          break;
      }
      break;
    }

    case 60: {
      switch (value) {
        case 0:
          mixer_1.gain(3, 0);
          waveformSub.amplitude(0);
          SubOctave = 0;
          break;
        case 1:
          mixer_1.gain(3, 0.5);
          ampSub.gain(defaultVolume_1);
          SubOctave = -12;
          break;
        case 2:
          mixer_1.gain(3, 0.5);
          ampSub.gain(defaultVolume_1);
          SubOctave = -24;
          break;
      }
      break;
    }    

    case 61: {
      if (value != SynthMode) {
        SynthMode = value;
        resetAllVoices();
      }
      break;
    }

    case 62: {
      GlobalBPM = (value + 60);
      break;
    } 

    case 63: { // Amplitude Attack
      float val = value * DIV127; // 0..1
      if (EGlockSelect == 0) {
        float attack = 3000.0f * val;
        envelope_Amplitude_1.attack(attack);
        envelope_Amplitude_2.attack(attack);
        envelope_Amplitude_3.attack(attack);
        envelopeSub_Amplitude.attack(attack);
        envelopeClone_Amplitude_1.attack(attack);
        envelopeClone_Amplitude_2.attack(attack);
        envelopeClone_Amplitude_3.attack(attack);
        envelopeNoise_1.attack(attack);
        DroneCarry_Amplitude_Attack_Value = attack;
      } else {
        int idx = (int)EGlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        float attack = subMs * val; // 0 .. subMs
        envelope_Amplitude_1.attack(attack);
        envelope_Amplitude_2.attack(attack);
        envelope_Amplitude_3.attack(attack);
        envelopeSub_Amplitude.attack(attack);
        envelopeClone_Amplitude_1.attack(attack);
        envelopeClone_Amplitude_2.attack(attack);
        envelopeClone_Amplitude_3.attack(attack);
        envelopeNoise_1.attack(attack);
        DroneCarry_Amplitude_Attack_Value = attack;
      }
      break;
    }

    case 64: { // Amplitude Decay
      float val = value * DIV127;
      if (EGlockSelect == 0) {
        float decay = 3000.0f * val;
        envelope_Amplitude_1.decay(decay);
        envelope_Amplitude_2.decay(decay);
        envelope_Amplitude_3.decay(decay);
        envelopeSub_Amplitude.decay(decay);
        envelopeClone_Amplitude_1.decay(decay);
        envelopeClone_Amplitude_2.decay(decay);
        envelopeClone_Amplitude_3.decay(decay);
        envelopeNoise_1.decay(decay);
        DroneCarry_Amplitude_Decay_Value = decay;
      } else {
        int idx = (int)EGlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        float decay = subMs * val;
        envelope_Amplitude_1.decay(decay);
        envelope_Amplitude_2.decay(decay);
        envelope_Amplitude_3.decay(decay);
        envelopeSub_Amplitude.decay(decay);
        envelopeClone_Amplitude_1.decay(decay);
        envelopeClone_Amplitude_2.decay(decay);
        envelopeClone_Amplitude_3.decay(decay);
        envelopeNoise_1.decay(decay);
        DroneCarry_Amplitude_Decay_Value = decay;
      }
      break;
    }

    case 65: { // Amplitude Sustain
      float sustain = value * DIV127;
      envelope_Amplitude_1.sustain(sustain);
      envelope_Amplitude_2.sustain(sustain);
      envelope_Amplitude_3.sustain(sustain);
      envelopeSub_Amplitude.sustain(sustain);
      envelopeClone_Amplitude_1.sustain(sustain);
      envelopeClone_Amplitude_2.sustain(sustain);
      envelopeClone_Amplitude_3.sustain(sustain);
      envelopeNoise_1.sustain(sustain);
      DroneCarry_Amplitude_Sustain_Value = sustain;
      break;
    }

    case 66: { // Amplitude Release
      float val = value * DIV127;
      if (EGlockSelect == 0) {
        float release = 3000.0f * val;
        envelope_Amplitude_1.release(release);
        envelope_Amplitude_2.release(release);
        envelope_Amplitude_3.release(release);
        envelopeSub_Amplitude.release(release);
        envelopeClone_Amplitude_1.release(release);
        envelopeClone_Amplitude_2.release(release);
        envelopeClone_Amplitude_3.release(release);
        envelopeNoise_1.release(release);
        DroneCarry_Amplitude_Release_Value = release;
      } else {
        int idx = (int)EGlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        float release = subMs * val;
        envelope_Amplitude_1.release(release);
        envelope_Amplitude_2.release(release);
        envelope_Amplitude_3.release(release);
        envelopeSub_Amplitude.release(release);
        envelopeClone_Amplitude_1.release(release);
        envelopeClone_Amplitude_2.release(release);
        envelopeClone_Amplitude_3.release(release);
        envelopeNoise_1.release(release);
        DroneCarry_Amplitude_Release_Value = release;
      }
      break;
    }

    case 67: { 
      // ladder cutoff --> map 0 .. 127 to freq 20 .. 10 000 Hz 
      LPFcutoffFreq = 10000.0 * (value * DIV127); 
      updateKBDtrackingMods();
      filterNoiseLP_1.frequency(LPFcutoffFreq);
      break; 
    }

    case 68: {
      KBDtracking = value * DIV127; // 0 .. 1 
      updateKBDtrackingMods();
      break;
    } 

    case 69: {
      // ladder resonance 0 .. 127 --> 0.0 .. 1.8
      LPFresonanceVal = 1.8 * (value * DIV127);
      ladder_1.resonance(LPFresonanceVal);   // usable resonance range is 0-1.8
      ladder_2.resonance(LPFresonanceVal);
      ladder_3.resonance(LPFresonanceVal);
      ladderSub.resonance(LPFresonanceVal);
      ladderClone_1.resonance(LPFresonanceVal);
      ladderClone_2.resonance(LPFresonanceVal);
      ladderClone_3.resonance(LPFresonanceVal);
      filterNoiseLP_1.resonance(0.7 + (LPFresonanceVal - 0.0) * (5.0 - 0.7) / (1.8 - 0.0)); // out = out_min + (in - in_min) * (out_max - out_min) / (in_max - in_min)
      break;
    }

    case 70: {
      // HPF cutoff
      HPFcutoffFreq = 20.0f * powf((10000.0f/20.0f), value * DIV127);     // logaritmic range 20 Hz - 10 kHz
      filterHP_1.frequency(HPFcutoffFreq);
      filterHP_2.frequency(HPFcutoffFreq);
      filterHP_3.frequency(HPFcutoffFreq);
      filterSubHP.frequency(HPFcutoffFreq);
      filterCloneHP_1.frequency(HPFcutoffFreq);
      filterCloneHP_2.frequency(HPFcutoffFreq);
      filterCloneHP_3.frequency(HPFcutoffFreq);
      filterNoiseHP_1.frequency(HPFcutoffFreq);
      break;
    }

    case 71: { // Filter Attack
      float val = value * DIV127;
      if (EGlockSelect == 0) {
        float attack = 3000.0f * val;
        Filter_Attack_Value = attack;
        DroneCarry_Filter_Attack_Value = attack;
      } else {
        int idx = (int)EGlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        float attack = subMs * val;
        Filter_Attack_Value = attack;
        DroneCarry_Filter_Attack_Value = attack;
      }
      break;
    }

    case 72: { // Filter Decay
      float val = value * DIV127;
      if (EGlockSelect == 0) {
        float decay = 3000.0f * val;
        Filter_Decay_Value = decay;
        DroneCarry_Filter_Decay_Value = decay;
      } else {
        int idx = (int)EGlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        float decay = subMs * val;
        Filter_Decay_Value = decay;
        DroneCarry_Filter_Decay_Value = decay;
      }
      break;
    }

    case 73: { // Filter Sustain (percentage)
      float sustain = value * DIV127;
      Filter_Sustain_Value = sustain;
      DroneCarry_Filter_Sustain_Value = sustain;
      break;
    }

    case 74: { // Filter Release
      float val = value * DIV127;
      if (EGlockSelect == 0) {
        float release = 3000.0f * val;
        Filter_Release_Value = release;
        DroneCarry_Filter_Release_Value = release;
      } else {
        int idx = (int)EGlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        float release = subMs * val;
        Filter_Release_Value = release;
        DroneCarry_Filter_Release_Value = release;
      }
      break;
    }

    case 75: {
      Filter_Intensity_Coeff = ((value * DIV127 * 2) - 1);          // -1 .. 1
      DroneCarry_Filter_Intensity_Coeff = ((value * DIV127 * 2) - 1);
      break;
    }

    case 76: {
      if (LFOlockSelect == 0) {
        float rate = value * DIV127;  // value is MIDI value 0–127
        float minFreq = 0.1f;         // 0.1 Hz (lowest speed)
        float maxFreq = 25.0f;        // 25 Hz (fastest speed)
        // Logaritmic mapping — smooth transitions
        float freq = minFreq * powf(maxFreq / minFreq, rate);
        // Convert frequency to time of one period in microseconds
        LFOrate = 1000000.0f / freq;  // µs
      } else {
        int idx = (int)LFOlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        if (subMs < 1.0f) subMs = 1.0f;
        LFOrate = subMs * 1000.0f; // µs
      }  
      break;
    }

    case 77: {
      LFOdepth = value * DIV127;
      break;
    }

    case 78: {
      float val = value * DIV127;
      if (LFOlockSelect == 0) {
        LFOdelay = 3000.0f * val; // ms, default
      } else {
        int idx = (int)LFOlockRateSelect;
        float subMs = subdivisionMsFromLockIndex(idx, GlobalBPM);
        const float delayMultiplier = 4.0f; // up to 4× subdiv as max delay
        LFOdelay = subMs * (val * delayMultiplier); // ms
      }
      break;
    }

    case 79: {
      LFOmodeSelect = value;
      resetLFOstate();   // always refresh off all voice LFO
      break;
    }

    case 80: {
      LFOtypeSelect = value;
        // update wave morphing
        updateMorphWaveform_1();
        updateMorphWaveform_2();
        updateMorphWaveform_3();
      break;
    }

    case CCLFOwaveSelect: {
      LFOwaveSelect = value;
      break;
    }

    case 82: {
      EGlockSelect = value;
      break;
    }

    case 83: {
      LFOlockSelect = value;
      break;
    }

    case 84: {
      EGlockRateSelect = value;
      break;
    }

    case 85: {
      LFOlockRateSelect = value;
      break;
    }

    case 86: {
      PORTswitch = value;
      if (PORTswitch == 1) {
        SynthMode = 0;
        resetAllVoices();
      }
      break;
    }

    case 87: {
      PORTmodeSelect = value;
      break;
    }

    case 88: {
      PORTlockRate = value;
      break;
    }

    case 89: {
      // MASTER volume 0 .. 127 -> 0 .. 1
      MASTERVolume = value * DIV127;
      amp_MASTER.gain(MASTERVolume);
      break;
    }

    case 90: {
      // enables unisonTriad toggle in UNISON SynthMode
      switch (value) {
        case 0:
          unisonTriadToggle = false;
          break;
        case 1:
          unisonTriadToggle = true;
          break;
      }
      break;
    }

    case 91: {
      // Pink volume 0 .. 127 -> 0 .. 1 -> 0 .. 0.75
      pinkVolume = value * DIV127 * 0.35f;
      pink_1.amplitude(pinkVolume);
      break;
    }

    case 92: {
      DRONEmodeSelect = value;
      break;
    }   

    case 93: {
      DRONEkeyNr1 = value;

      if (value == 1) { // tlačítko stisknuto
          if (DRONEmodeSelect == 1) {
            // request: uložení snapshotu - DEFER do loop() kvůli bezpečnosti
            DRONE_requestSave[0] = true;
          } else { // DRONEmodeSelect == 0 -> hold/play mode -> spustit uložený snapshot
            DronePlaySlot(0);      // nebo DroneKeyOnSlot(slot) — dle názvu v drone souborech
          }
      } else { // value == 0 -> tlačítko uvolněno (v hold/play módu uvolnit)
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(0);
        }
          // v save módu nic při uvolnění
      }
      break;
    }
    
    case 94: {
      DRONEkeyNr2 = value;

      if (value == 1) {
          if (DRONEmodeSelect == 1) {
            DRONE_requestSave[1] = true;
          } else {
            DronePlaySlot(1);
          }
      } else {
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(1);
        }
      }
      break;
    }
    
    case 95: {
      DRONEkeyNr3 = value;

      if (value == 1) {
          if (DRONEmodeSelect == 1) {
            DRONE_requestSave[2] = true;
          } else {
            DronePlaySlot(2);
          }
      } else {
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(2);
        }
      }
      break;
    }

    case 96: {
      DRONEkeyNr4 = value;

      if (value == 1) {
          if (DRONEmodeSelect == 1) {
            DRONE_requestSave[3] = true;
          } else {
            DronePlaySlot(3);
          }
      } else {
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(3);
        }
      }
      break;
    }

    case 97: {
      DRONEkeyNr5 = value;

      if (value == 1) {
          if (DRONEmodeSelect == 1) {
            DRONE_requestSave[4] = true;
          } else {
            DronePlaySlot(4);
          }
      } else {
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(4);
        }
      }
      break;
    }

    case 98: {
      DRONEkeyNr6 = value;

      if (value == 1) {
          if (DRONEmodeSelect == 1) {
            DRONE_requestSave[5] = true;
          } else {
            DronePlaySlot(5);
          }
      } else {
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(5);
        }
      }
      break;
    }

    case 99: {
      DRONEkeyNr7 = value;

      if (value == 1) {
          if (DRONEmodeSelect == 1) {
            DRONE_requestSave[6] = true;
          } else {
            DronePlaySlot(6);
          }
      } else {
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(6);
        }
      }
      break;
    }

    case 100: {
      DRONEkeyNr8 = value;

      if (value == 1) {
          if (DRONEmodeSelect == 1) {
            DRONE_requestSave[7] = true;
          } else {
            DronePlaySlot(7);
          }
      } else {
        if (DRONEmodeSelect == 0) {
          DroneStopSlot(7);
        }
      }
      break;
    }   
  }
}