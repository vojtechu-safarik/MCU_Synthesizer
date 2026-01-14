// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\Drone.h"
#include "Synthesis\LFO.h"
#include "Synthesis\KBD_Tracking.h"
#include "Synthesis\Filter_Envelope.h"
#include "Synthesis\ShapeDetuneRecomputeFix.h"
#include "Synthesis\Portamento.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\updateVoices.h"
#include "Voices\oscStopVoice.h"
// ==================

void updateVoices() {
  updateEnvelopesPerVoice();
  PortamentoUpdate();
  ShapeDetuneRecomputeFix();
  if (SynthMode == 0) {
    // UNISONO
    if ((unisonTriad == 0) || (voiceReleasing_1 == true)) { // Default Triad
      if (voiceActive_4 == true) {
        oscStopVoice_1();
        oscStopVoice_2();
        oscStopVoice_3();
        envelopeNoise_1.noteOff();
      }
      waveform_1.frequency(noteFreqs[voiceNote_1 + octave] * Tune_1 * PORTdetune_1);
      waveform_2.frequency(noteFreqs[voiceNote_2 + octave] * Tune_2 * PORTdetune_2);
      waveform_3.frequency(noteFreqs[voiceNote_3 + octave] * Tune_3 * PORTdetune_3);
      waveform_1.amplitude(defaultVolume_1);
      waveform_2.amplitude(defaultVolume_2);
      waveform_3.amplitude(defaultVolume_3);

      float baseCut1 = computeVoiceLPFCutoff(voiceNote_1 + octave);
      float baseCut2 = computeVoiceLPFCutoff(voiceNote_2 + octave);
      float baseCut3 = computeVoiceLPFCutoff(voiceNote_3 + octave);
      float finalCut1 = baseCut1 * LFOfreqCutoffMod_1 * filterEnvelope_Coefficient[0];
      float finalCut2 = baseCut2 * LFOfreqCutoffMod_2 * filterEnvelope_Coefficient[1];
      float finalCut3 = baseCut3 * LFOfreqCutoffMod_3 * filterEnvelope_Coefficient[2];
      
      DroneA_finalCutoff1 = finalCut1;
      DroneA_finalCutoff2 = finalCut2;
      DroneA_finalCutoff3 = finalCut3;
      DroneB_finalCutoff1 = finalCut1;
      DroneB_finalCutoff2 = finalCut2;
      DroneB_finalCutoff3 = finalCut3;      

      ladder_1.frequency(finalCut1);
      ladder_2.frequency(finalCut2);
      ladder_3.frequency(finalCut3);

      if (!envelope_Amplitude_1.isActive() && voiceReleasing_1) {
        waveform_1.amplitude(0);
        waveform_2.amplitude(0);
        waveform_3.amplitude(0);
        voiceActive_1 = false;
        voiceActive_2 = false;
        voiceActive_3 = false;
        voiceReleasing_1 = false;
        voiceReleasing_2 = false;
        voiceReleasing_3 = false;
      }
    }

    if (unisonTriad == 0) {
      waveformSub.frequency(noteFreqs[voiceNote_1 + octave + SubOctave] * Tune_1 * PORTdetune_1);
      waveformSub.amplitude(defaultVolume_1);
      float baseCut70 = computeVoiceLPFCutoff(voiceNote_1 + octave + SubOctave);
      float finalCut70 = baseCut70 * LFOfreqCutoffMod_7 * filterEnvelope_Coefficient[0];

      DroneA_finalCutoff70 = finalCut70;
      DroneB_finalCutoff70 = finalCut70;

      ladderSub.frequency(finalCut70);
    }

    if ((unisonTriad == 1) || (voiceReleasing_4 == true)) { // Clone Triad
      if (voiceActive_1 == true) {
        oscStopVoice_4();
        oscStopVoice_5();
        oscStopVoice_6();
        envelopeNoise_1.noteOff();
      }    
      waveformClone_1.frequency(noteFreqs[voiceNote_4 + octave] * Tune_4 * PORTdetune_4);
      waveformClone_2.frequency(noteFreqs[voiceNote_5 + octave] * Tune_5 * PORTdetune_5);
      waveformClone_3.frequency(noteFreqs[voiceNote_6 + octave] * Tune_6 * PORTdetune_6);
      waveformClone_1.amplitude(defaultVolume_1);
      waveformClone_2.amplitude(defaultVolume_2);
      waveformClone_3.amplitude(defaultVolume_3);

      float baseCut4 = computeVoiceLPFCutoff(voiceNote_4 + octave);
      float baseCut5 = computeVoiceLPFCutoff(voiceNote_5 + octave);
      float baseCut6 = computeVoiceLPFCutoff(voiceNote_6 + octave);
      float finalCut4 = baseCut4 * LFOfreqCutoffMod_4 * filterEnvelope_Coefficient[3];
      float finalCut5 = baseCut5 * LFOfreqCutoffMod_5 * filterEnvelope_Coefficient[4];
      float finalCut6 = baseCut6 * LFOfreqCutoffMod_6 * filterEnvelope_Coefficient[5];


      DroneA_finalCutoff4 = finalCut4;
      DroneA_finalCutoff5 = finalCut5;
      DroneA_finalCutoff6 = finalCut6;
      DroneB_finalCutoff4 = finalCut4;
      DroneB_finalCutoff5 = finalCut5;
      DroneB_finalCutoff6 = finalCut6;

      ladderClone_1.frequency(finalCut4);
      ladderClone_2.frequency(finalCut5);
      ladderClone_3.frequency(finalCut6);

      if (!envelopeClone_Amplitude_1.isActive() && voiceReleasing_4) {
        waveformClone_1.amplitude(0);
        waveformClone_2.amplitude(0);
        waveformClone_3.amplitude(0);
        voiceActive_4 = false;
        voiceActive_5 = false;
        voiceActive_6 = false;
        voiceReleasing_4 = false;
        voiceReleasing_5 = false;
        voiceReleasing_6 = false;
      }
    }

    if (unisonTriad == 1) {
      waveformSub.frequency(noteFreqs[voiceNote_4 + octave + SubOctave] * Tune_4 * PORTdetune_4);
      waveformSub.amplitude(defaultVolume_1);
      float baseCut71 = computeVoiceLPFCutoff(voiceNote_4 + octave + SubOctave);
      float finalCut71 = baseCut71 * LFOfreqCutoffMod_7 * filterEnvelope_Coefficient[3];

      DroneA_finalCutoff71 = finalCut71;
      DroneB_finalCutoff71 = finalCut71;

      ladderSub.frequency(finalCut71);
    }

  } else if (SynthMode == 1) {
    // POLY
    // Voice 1 (apply even when releasing, so that the release is modulated by LFO correctly)
    if (voiceActive_1 || voiceReleasing_1) {
      waveform_1.frequency(noteFreqs[voiceNote_1 + octave] * Tune_1);
      waveformSub.frequency(noteFreqs[voiceNote_1 + octave + SubOctave] * Tune_1);
      waveform_1.amplitude(defaultVolume_1);
      waveformSub.amplitude(defaultVolume_1);

      float baseCut1 = computeVoiceLPFCutoff(voiceNote_1 + octave);
      float baseCut7 = computeVoiceLPFCutoff(voiceNote_1 + octave + SubOctave);
      float finalCut1 = baseCut1 * LFOfreqCutoffMod_1 * filterEnvelope_Coefficient[0];
      float finalCut7 = baseCut7 * LFOfreqCutoffMod_1 * filterEnvelope_Coefficient[0];
      ladder_1.frequency(finalCut1);
      ladderSub.frequency(finalCut7);

      if (!envelope_Amplitude_1.isActive() && voiceReleasing_1) {
        waveform_1.amplitude(0);
        waveformSub.amplitude(0);
        voiceActive_1 = false;
        voiceReleasing_1 = false;
      }
    }

    // Voice 2
    if (voiceActive_2 || voiceReleasing_2) {
      waveform_2.frequency(noteFreqs[voiceNote_2 + octave] * Tune_2);
      waveform_2.amplitude(defaultVolume_2);

      float baseCut2 = computeVoiceLPFCutoff(voiceNote_2 + octave);
      float finalCut2 = baseCut2 * LFOfreqCutoffMod_2 * filterEnvelope_Coefficient[1];
      ladder_2.frequency(finalCut2);

      if (!envelope_Amplitude_2.isActive() && voiceReleasing_2) {
        waveform_2.amplitude(0);
        voiceActive_2 = false;
        voiceReleasing_2 = false;
      }
    }

    // Voice 3
    if (voiceActive_3 || voiceReleasing_3) {
      waveform_3.frequency(noteFreqs[voiceNote_3 + octave] * Tune_3);
      waveform_3.amplitude(defaultVolume_3);

      float baseCut3 = computeVoiceLPFCutoff(voiceNote_3 + octave);
      float finalCut3 = baseCut3 * LFOfreqCutoffMod_3 * filterEnvelope_Coefficient[2];
      ladder_3.frequency(finalCut3);

      if (!envelope_Amplitude_3.isActive() && voiceReleasing_3) {
        waveform_3.amplitude(0);
        voiceActive_3 = false;
        voiceReleasing_3 = false;
      }
    }

    // Voice 4
    if (voiceActive_4 || voiceReleasing_4) {
      waveformClone_1.frequency(noteFreqs[voiceNote_4 + octave] * Tune_4);
      waveformClone_1.amplitude(defaultVolume_3);

      float baseCut4 = computeVoiceLPFCutoff(voiceNote_4 + octave);
      float finalCut4 = baseCut4 * LFOfreqCutoffMod_4 * filterEnvelope_Coefficient[3];
      ladderClone_1.frequency(finalCut4);

      if (!envelopeClone_Amplitude_1.isActive() && voiceReleasing_4) {
        waveformClone_1.amplitude(0);
        voiceActive_4 = false;
        voiceReleasing_4 = false;
      }
    }

    // Voice 5
    if (voiceActive_5 || voiceReleasing_5) {
      waveformClone_2.frequency(noteFreqs[voiceNote_5 + octave] * Tune_5);
      waveformClone_2.amplitude(defaultVolume_3);

      float baseCut5 = computeVoiceLPFCutoff(voiceNote_5 + octave);
      float finalCut5 = baseCut5 * LFOfreqCutoffMod_5 * filterEnvelope_Coefficient[4];
      ladderClone_2.frequency(finalCut5);

      if (!envelopeClone_Amplitude_2.isActive() && voiceReleasing_5) {
        waveformClone_2.amplitude(0);
        voiceActive_5 = false;
        voiceReleasing_5 = false;
      }
    }

    // Voice 6
    if (voiceActive_6 || voiceReleasing_6) {
      waveformClone_3.frequency(noteFreqs[voiceNote_6 + octave] * Tune_6);
      waveformClone_3.amplitude(defaultVolume_3);

      float baseCut6 = computeVoiceLPFCutoff(voiceNote_6 + octave);
      float finalCut6 = baseCut6 * LFOfreqCutoffMod_6 * filterEnvelope_Coefficient[5];
      ladderClone_3.frequency(finalCut6);

      if (!envelopeClone_Amplitude_3.isActive() && voiceReleasing_6) {
        waveformClone_3.amplitude(0);
        voiceActive_6 = false;
        voiceReleasing_6 = false;
      }
    }
  }
}