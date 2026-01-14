// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
#include "Global_Variables\Wavetable_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\BPMLock.h"
#include "Synthesis\Drone.h"
// ==================

// === CONTROLS ===
#include "Controls\MIDI_Control.h"
// ==================

// ==== DRONE ====
// Helper: find snapshot index (0..7) by MIDI note number (DRONEkeyNr1..8)
static int findSnapshotIndex(byte note) {
    if (note == DRONEkeyNr1) return 0;
    if (note == DRONEkeyNr2) return 1;
    if (note == DRONEkeyNr3) return 2;
    if (note == DRONEkeyNr4) return 3;
    if (note == DRONEkeyNr5) return 4;
    if (note == DRONEkeyNr6) return 5;
    if (note == DRONEkeyNr7) return 6;
    if (note == DRONEkeyNr8) return 7;
    return -1;
}

// Save current synth settings into a drone snapshot
void DroneSaveValues(int snapshotIndex) {
    if (snapshotIndex < 0 || snapshotIndex >= DRONE_SLOTS) return;
    DroneSnapshot &snap = droneSnapshots[snapshotIndex];

    // Save filter and HP settings (resolved current values)
    snap.cutoff = LPFcutoffFreq;
    snap.resonance = 0.7 + (LPFresonanceVal - 0.0) * (5.0 - 0.7) / (1.8 - 0.0); // out = out_min + (in - in_min) * (out_max - out_min) / (in_max - in_min)
    snap.hpfFreq = HPFcutoffFreq;
    snap.hpfRes = defaultHpfRes;

    // Copy the morph result waveforms (primary three) into pool
    memcpy(droneWaveforms[snapshotIndex][0], myWaveform_1, sizeof(myWaveform_1));
    memcpy(droneWaveforms[snapshotIndex][1], myWaveform_2, sizeof(myWaveform_2));
    memcpy(droneWaveforms[snapshotIndex][2], myWaveform_3, sizeof(myWaveform_3));

    // Save which waveFamily & shape index were active at save-time (for faithful shape-LFO)
    snap.savedWaveFamilyIndex[0] = (uint8_t)currentWaveFamily_1;
    snap.savedShapeIndex[0]     = currentShapeIndex_1;

    snap.savedWaveFamilyIndex[1] = (uint8_t)currentWaveFamily_2;
    snap.savedShapeIndex[1]     = currentShapeIndex_2;

    snap.savedWaveFamilyIndex[2] = (uint8_t)currentWaveFamily_3;
    snap.savedShapeIndex[2]     = currentShapeIndex_3;

    // save the Volume values to snapshot (so that they dont depend on global value)
    snap.savedVolume1 = defaultVolume_1;
    snap.savedVolume2 = defaultVolume_2;
    snap.savedVolume3 = defaultVolume_3;
    snap.savedNoiseVolume = pinkVolume;
    snap.savedSubOctave = SubOctave;

    // Reset snapshot voice flags/notes
    for (int i = 0; i < 5; i++) {
        snap.voiceActive[i] = false;
        snap.note[i] = 0;
    }

    // Save ADSR/filter carry values if you have them (example variables)
    snap.envAmpAttack  = DroneCarry_Amplitude_Attack_Value;
    snap.envAmpDecay   = DroneCarry_Amplitude_Decay_Value;
    snap.envAmpSustain = DroneCarry_Amplitude_Sustain_Value;
    snap.envAmpRelease = DroneCarry_Amplitude_Release_Value;

    snap.envFilterAttack  = DroneCarry_Filter_Attack_Value;
    snap.envFilterDecay   = DroneCarry_Filter_Decay_Value;
    snap.envFilterSustain = DroneCarry_Filter_Sustain_Value;
    snap.envFilterRelease = DroneCarry_Filter_Release_Value;
    snap.envFilterIntensity = DroneCarry_Filter_Intensity_Coeff;

    // --- Uložit LFO nastavení do snapshotu ---
    snap.lfoRate_us    = LFOrate;        // main LFOrate (µs per period)
    snap.lfoDepth      = LFOdepth;       // 0..1
    snap.lfoWaveSelect = LFOwaveSelect;  // 0=sine,1=saw,2=square
    snap.lfoModeSelect = LFOmodeSelect;  // 0=FREE,1=RETRIG,2=RISE,3=FALL
    snap.lfoTypeSelect = LFOtypeSelect;  // 0=freq(detune),1=shape,2=cutoff
    
    snap.lfoDelay_ms = (unsigned int)(LFOdelay);
    // Different saving strategy for UNISON vs POLY
    int idx = 0;
    if (SynthMode == 0) {
        // UNISON: find which triad is currently active (1..3 or 4..6).
        // if any of voices 4..6 active, assume clone triad active.
        bool cloneTriadActive = (voiceActive_4 || voiceActive_5 || voiceActive_6);
        if (!cloneTriadActive) {
            // use voices 1..3
            if (voiceActive_1 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_1 + octave, 0, 127); idx++; }
            if (voiceActive_2 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_2 + octave, 0, 127); idx++; }
            if (voiceActive_3 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_3 + octave, 0, 127); idx++; }
            // sub oscillator copies voice 1 if present
            if (voiceActive_SUB && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_1 + octave + SubOctave, 0, 127); idx++; }
            // 5th voice intentionally left unused in unison
        } else {
            // use voices 4..6 (clone triad)
            if (voiceActive_4 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_4 + octave, 0, 127); idx++; }
            if (voiceActive_5 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_5 + octave, 0, 127); idx++; }
            if (voiceActive_6 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_6 + octave, 0, 127); idx++; }
            // sub oscillator in clone triad also copies voice 4 if present
            if (voiceActive_SUB && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_4 + octave + SubOctave, 0, 127); idx++; }
            // 5th voice left unused
        }
    } else {
        // POLY: copy up to 5 active voices in order 1..6
        if (voiceActive_1 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_1 + octave, 0, 127); idx++; }
        if (voiceActive_2 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_2 + octave, 0, 127); idx++; }
        if (voiceActive_3 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_3 + octave, 0, 127); idx++; }
        if (voiceActive_4 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_4 + octave, 0, 127); idx++; }
        if (voiceActive_5 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_5 + octave, 0, 127); idx++; }
        if (voiceActive_6 && idx < 5) { snap.voiceActive[idx] = true; snap.note[idx] = (byte)constrain(voiceNote_6 + octave, 0, 127); idx++; }
        // no special sub-handling here (sub handled by KBDtrackingMod_7 etc)
    }

    // mark as valid and record timestamp
    snap.valid = true;
    snap.savedTimestamp = millis();
}

static void playSnapshot(int snapIndex, bool useGroupA) {
    if (snapIndex < 0 || snapIndex >= DRONE_SLOTS) return;
    DroneSnapshot &snap = droneSnapshots[snapIndex];

    // safety: if snapshot is not valid, do nothing
    if (!snap.valid) return;

    // --- CANCEL ONGOING RELEASE FOR THIS SNAPSHOT so re-press restarts attack immediately
    // clear per-snapshot release bookkeeping (safe to do even if not releasing)
    droneReleaseStart[snapIndex] = 0;
    drone_savedAttackCoef[snapIndex]  = 1.0f;
    drone_savedDecayCoef[snapIndex]   = 1.0f;
    drone_savedSustainCoef[snapIndex] = 1.0f;
    droneFilter_Attack_Coeff[snapIndex]   = 1.0f;
    droneFilter_Decay_Coeff[snapIndex]    = 1.0f;
    droneFilter_Sustain_Coeff[snapIndex]  = 1.0f;
    droneFilter_Release_Coeff[snapIndex]  = 1.0f;
    droneFilterEnvelope_Coeff[snapIndex]  = 1.0f;
    dronePrevReleasing[snapIndex] = false;

    // If this snapshot was marked as releasing via group flags, clear those group flags too.
    // (if some other group really is releasing a different snapshot, this does not affect it)
    if (releasingDroneA_index == snapIndex) { droneGroupA_releasing = false; releasingDroneA_index = -1; }
    if (releasingDroneB_index == snapIndex) { droneGroupB_releasing = false; releasingDroneB_index = -1; }


    // Copy LFO parameters from snapshot -> runtime (per-group)
    if (useGroupA) {
        // copy settings
        DroneA_LFOrate_us     = snap.lfoRate_us;
        DroneA_LFOdepth       = snap.lfoDepth;
        DroneA_LFOwaveSelect  = snap.lfoWaveSelect;
        DroneA_LFOmodeSelect  = snap.lfoModeSelect;
        DroneA_LFOtypeSelect  = snap.lfoTypeSelect;
        DroneA_LFOdelay_ms    = snap.lfoDelay_ms;

        // initialize runtime state depending on mode/delay
        if (DroneA_LFOdelay_ms > 0) {
            DroneA_LFOdelayEndMillis = millis() + DroneA_LFOdelay_ms;
            DroneA_LFOactive = false;
            // set initial once/phase values consistent with mode
            if (DroneA_LFOmodeSelect == 2) { 
              DroneA_LFOonce = 0.0f; 
              DroneA_LFOstop = false; 
            }
            else if (DroneA_LFOmodeSelect == 3) { 
              DroneA_LFOonce = 1.0f; 
              DroneA_LFOstop = false; 
            }
            else { 
              DroneA_LFOphase = 0.0f; 
              DroneA_LFOstop = false; 
            }
        } else {
            DroneA_LFOdelayEndMillis = 0;
            if (DroneA_LFOmodeSelect == 1) { // RETRIG
                DroneA_LFOphase = 0.0f;
                DroneA_LFOactive = true;
                DroneA_LFOstop = false;
            } else if (DroneA_LFOmodeSelect == 2) { // RISE
                DroneA_LFOonce = 0.0f;
                DroneA_LFOactive = true;
                DroneA_LFOstop = false;
            } else if (DroneA_LFOmodeSelect == 3) { // FALL
                DroneA_LFOonce = 1.0f;
                DroneA_LFOactive = true;
                DroneA_LFOstop = false;
            } else { // FREE
                DroneA_LFOactive = true;
                DroneA_LFOstop = false;
            }
        }
    } else {
        // Group B copy
        DroneB_LFOrate_us     = snap.lfoRate_us;
        DroneB_LFOdepth       = snap.lfoDepth;
        DroneB_LFOwaveSelect  = snap.lfoWaveSelect;
        DroneB_LFOmodeSelect  = snap.lfoModeSelect;
        DroneB_LFOtypeSelect  = snap.lfoTypeSelect;
        DroneB_LFOdelay_ms    = snap.lfoDelay_ms;

        if (DroneB_LFOdelay_ms > 0) {
            DroneB_LFOdelayEndMillis = millis() + DroneB_LFOdelay_ms;
            DroneB_LFOactive = false;
            if (DroneB_LFOmodeSelect == 2) { 
              DroneB_LFOonce = 0.0f; 
              DroneB_LFOstop = false; 
            }
            else if (DroneB_LFOmodeSelect == 3) { 
              DroneB_LFOonce = 1.0f; 
              DroneB_LFOstop = false; 
            }
            else { 
              DroneB_LFOphase = 0.0f; 
              DroneB_LFOstop = false; 
            }
        } else {
            DroneB_LFOdelayEndMillis = 0;
            if (DroneB_LFOmodeSelect == 1) { 
              DroneB_LFOphase = 0.0f; 
              DroneB_LFOactive = true; 
              DroneB_LFOstop = false; 
            }
            else if (DroneB_LFOmodeSelect == 2) { 
              DroneB_LFOonce = 0.0f; 
              DroneB_LFOactive = true; 
              DroneB_LFOstop = false; 
            }
            else if (DroneB_LFOmodeSelect == 3) { 
              DroneB_LFOonce = 1.0f; 
              DroneB_LFOactive = true; 
              DroneB_LFOstop = false; 
            }
            else { 
              DroneB_LFOactive = true; 
              DroneB_LFOstop = false; 
            }
        }
    }

    // For each of the 5 potential voices in the group:
    for (int i = 0; i < 5; ++i) {
        bool activeHere = snap.voiceActive[i];

        // UNISON special handling: sub (index 3) copies first triad, index 4 unused
        if (SynthMode == 0) {
            if (i == 3) { activeHere = snap.voiceActive[0]; } // sub active only if first triad present
            if (i == 4) { activeHere = false; } // unused in unison
        }

        // If not active -> silence
        if (!activeHere) {
            if (useGroupA) {
                switch (i) {
                    case 0: waveformDroneA_1.amplitude(0); break;
                    case 1: waveformDroneA_2.amplitude(0); break;
                    case 2: waveformDroneA_3.amplitude(0); break;
                    case 3: waveformDroneA_4.amplitude(0); break;
                    case 4: waveformDroneA_5.amplitude(0); break;
                }
            } else {
                switch (i) {
                    case 0: waveformDroneB_1.amplitude(0); break;
                    case 1: waveformDroneB_2.amplitude(0); break;
                    case 2: waveformDroneB_3.amplitude(0); break;
                    case 3: waveformDroneB_4.amplitude(0); break;
                    case 4: waveformDroneB_5.amplitude(0); break;
                }
            }
            continue;
        }

        // determine midi-note index for this saved voice (respect unison sub)
        int noteIndex = 0;
        if (SynthMode == 0) {
            // UNISON: triad mapping saved in snap.note[0..2]
            if (i <= 2) {
                noteIndex = (int)snap.note[i];
            } else if (i == 3) {
                // sub: take snap.note[0] and apply savedSubOctave
                noteIndex = (int)snap.note[0] + snap.savedSubOctave;
            } else {
                noteIndex = (int)snap.note[i]; // fallback
            }
        } else {
            // POLY: direct mapping
            noteIndex = (int)snap.note[i];
        }
        // clamp and compute base frequency
        if (noteIndex < 0) noteIndex = 0;
        if (noteIndex > 127) noteIndex = 127;
        float freq = noteFreqs[noteIndex];

        // select waveform index (0..DRONE_WAVE_VOICES-1). voices >= DRONE_WAVE_VOICES inherit last index
        int wfIdx = (i < DRONE_WAVE_VOICES) ? i : (DRONE_WAVE_VOICES - 1);

        // amplitude mapping depending on SynthMode + saved volumes
        float amp = 0.0f;
        if (SynthMode == 0) {
            // UNISON mapping: voice 0 -> savedVolume1, voice1 -> savedVolume2, voice2 -> savedVolume3
            // sub (i==3) uses savedVolume1 (copied from voice1 triad)
            if (i == 0) amp = snap.savedVolume1;
            else if (i == 1) amp = snap.savedVolume2;
            else if (i == 3) amp = snap.savedVolume1;
            else amp = snap.savedVolume3; // i==2 or fallback
        } else {
            // POLY mapping: 0->v1,1->v2,2->v3, >=2 -> v3
            if (i == 0) amp = snap.savedVolume1;
            else if (i == 1) amp = snap.savedVolume2;
            else amp = snap.savedVolume3;
        }

        // Apply waveform, frequency, amplitude, envelopes and filters per group
        if (useGroupA) {
            switch (i) {
                case 0:
                    waveformDroneA_1.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneA_1.frequency(freq);
                    waveformDroneA_1.amplitude(0.25);
                    ampDroneA_1.gain(amp);
                    SV_ladderDroneA_1.frequency(snap.cutoff);
                    SV_ladderDroneA_1.resonance(snap.resonance);
                    filterDroneAHP_1.frequency(snap.hpfFreq);
                    envelopeDroneA_Amplitude_1.attack(snap.envAmpAttack);
                    envelopeDroneA_Amplitude_1.decay(snap.envAmpDecay);
                    envelopeDroneA_Amplitude_1.sustain(snap.envAmpSustain);
                    envelopeDroneA_Amplitude_1.release(snap.envAmpRelease);
                    envelopeDroneA_Amplitude_1.noteOn();

                    pinkDroneA.amplitude(snap.savedNoiseVolume);
                    filterNoiseDroneALP.frequency(snap.cutoff);
                    filterNoiseDroneALP.resonance(snap.resonance);
                    filterNoiseDroneAHP.frequency(snap.hpfFreq);
                    envelopeNoiseDroneA.attack(snap.envAmpAttack);
                    envelopeNoiseDroneA.decay(snap.envAmpDecay);
                    envelopeNoiseDroneA.sustain(snap.envAmpSustain);
                    envelopeNoiseDroneA.release(snap.envAmpRelease);
                    envelopeNoiseDroneA.noteOn();
                    break;
                case 1:
                    waveformDroneA_2.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneA_2.frequency(freq);
                    waveformDroneA_2.amplitude(0.25);
                    ampDroneA_2.gain(amp);
                    SV_ladderDroneA_2.frequency(snap.cutoff);
                    SV_ladderDroneA_2.resonance(snap.resonance);
                    filterDroneAHP_2.frequency(snap.hpfFreq);
                    envelopeDroneA_Amplitude_2.attack(snap.envAmpAttack);
                    envelopeDroneA_Amplitude_2.decay(snap.envAmpDecay);
                    envelopeDroneA_Amplitude_2.sustain(snap.envAmpSustain);
                    envelopeDroneA_Amplitude_2.release(snap.envAmpRelease);
                    envelopeDroneA_Amplitude_2.noteOn();
                    break;
                case 2:
                    waveformDroneA_3.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneA_3.frequency(freq);
                    waveformDroneA_3.amplitude(0.25);
                    ampDroneA_3.gain(amp);
                    SV_ladderDroneA_3.frequency(snap.cutoff);
                    SV_ladderDroneA_3.resonance(snap.resonance);
                    filterDroneAHP_3.frequency(snap.hpfFreq);
                    envelopeDroneA_Amplitude_3.attack(snap.envAmpAttack);
                    envelopeDroneA_Amplitude_3.decay(snap.envAmpDecay);
                    envelopeDroneA_Amplitude_3.sustain(snap.envAmpSustain);
                    envelopeDroneA_Amplitude_3.release(snap.envAmpRelease);
                    envelopeDroneA_Amplitude_3.noteOn();
                    break;
                case 3:
                    waveformDroneA_4.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneA_4.frequency(freq);
                    waveformDroneA_4.amplitude(0.25);
                    ampDroneA_4.gain(amp);
                    SV_ladderDroneA_4.frequency(snap.cutoff);
                    SV_ladderDroneA_4.resonance(snap.resonance);
                    filterDroneAHP_4.frequency(snap.hpfFreq);
                    envelopeDroneA_Amplitude_4.attack(snap.envAmpAttack);
                    envelopeDroneA_Amplitude_4.decay(snap.envAmpDecay);
                    envelopeDroneA_Amplitude_4.sustain(snap.envAmpSustain);
                    envelopeDroneA_Amplitude_4.release(snap.envAmpRelease);
                    envelopeDroneA_Amplitude_4.noteOn();
                    break;
                case 4:
                    waveformDroneA_5.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneA_5.frequency(freq);
                    waveformDroneA_5.amplitude(0.25);
                    ampDroneA_5.gain(amp);
                    SV_ladderDroneA_5.frequency(snap.cutoff);
                    SV_ladderDroneA_5.resonance(snap.resonance);
                    filterDroneAHP_5.frequency(snap.hpfFreq);
                    envelopeDroneA_Amplitude_5.attack(snap.envAmpAttack);
                    envelopeDroneA_Amplitude_5.decay(snap.envAmpDecay);
                    envelopeDroneA_Amplitude_5.sustain(snap.envAmpSustain);
                    envelopeDroneA_Amplitude_5.release(snap.envAmpRelease);
                    envelopeDroneA_Amplitude_5.noteOn();
                    break;
            }
        } else {
            // Group B (analogical assignments)
            switch (i) {
                case 0:
                    waveformDroneB_1.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneB_1.frequency(freq);
                    waveformDroneB_1.amplitude(0.25);
                    ampDroneB_1.gain(amp);
                    SV_ladderDroneB_1.frequency(snap.cutoff);
                    SV_ladderDroneB_1.resonance(snap.resonance);
                    filterDroneBHP_1.frequency(snap.hpfFreq);
                    envelopeDroneB_Amplitude_1.attack(snap.envAmpAttack);
                    envelopeDroneB_Amplitude_1.decay(snap.envAmpDecay);
                    envelopeDroneB_Amplitude_1.sustain(snap.envAmpSustain);
                    envelopeDroneB_Amplitude_1.release(snap.envAmpRelease);
                    envelopeDroneB_Amplitude_1.noteOn();

                    pinkDroneB.amplitude(snap.savedNoiseVolume);
                    filterNoiseDroneBLP.frequency(snap.cutoff);
                    filterNoiseDroneBLP.resonance(snap.resonance);
                    filterNoiseDroneBHP.frequency(snap.hpfFreq);
                    envelopeNoiseDroneB.attack(snap.envAmpAttack);
                    envelopeNoiseDroneB.decay(snap.envAmpDecay);
                    envelopeNoiseDroneB.sustain(snap.envAmpSustain);
                    envelopeNoiseDroneB.release(snap.envAmpRelease);
                    envelopeNoiseDroneB.noteOn();
                    break;
                case 1:
                    waveformDroneB_2.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneB_2.frequency(freq);
                    waveformDroneB_2.amplitude(0.25);
                    ampDroneB_2.gain(amp);
                    SV_ladderDroneB_2.frequency(snap.cutoff);
                    SV_ladderDroneB_2.resonance(snap.resonance);
                    filterDroneBHP_2.frequency(snap.hpfFreq);
                    envelopeDroneB_Amplitude_2.attack(snap.envAmpAttack);
                    envelopeDroneB_Amplitude_2.decay(snap.envAmpDecay);
                    envelopeDroneB_Amplitude_2.sustain(snap.envAmpSustain);
                    envelopeDroneB_Amplitude_2.release(snap.envAmpRelease);
                    envelopeDroneB_Amplitude_2.noteOn();
                    break;
                case 2:
                    waveformDroneB_3.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneB_3.frequency(freq);
                    waveformDroneB_3.amplitude(0.25);
                    ampDroneB_3.gain(amp);
                    SV_ladderDroneB_3.frequency(snap.cutoff);
                    SV_ladderDroneB_3.resonance(snap.resonance);
                    filterDroneBHP_3.frequency(snap.hpfFreq);
                    envelopeDroneB_Amplitude_3.attack(snap.envAmpAttack);
                    envelopeDroneB_Amplitude_3.decay(snap.envAmpDecay);
                    envelopeDroneB_Amplitude_3.sustain(snap.envAmpSustain);
                    envelopeDroneB_Amplitude_3.release(snap.envAmpRelease);
                    envelopeDroneB_Amplitude_3.noteOn();
                    break;
                case 3:
                    waveformDroneB_4.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneB_4.frequency(freq);
                    waveformDroneB_4.amplitude(0.25);
                    ampDroneB_4.gain(amp);
                    SV_ladderDroneB_4.frequency(snap.cutoff);
                    SV_ladderDroneB_4.resonance(snap.resonance);
                    filterDroneBHP_4.frequency(snap.hpfFreq);
                    envelopeDroneB_Amplitude_4.attack(snap.envAmpAttack);
                    envelopeDroneB_Amplitude_4.decay(snap.envAmpDecay);
                    envelopeDroneB_Amplitude_4.sustain(snap.envAmpSustain);
                    envelopeDroneB_Amplitude_4.release(snap.envAmpRelease);
                    envelopeDroneB_Amplitude_4.noteOn();
                    break;
                case 4:
                    waveformDroneB_5.arbitraryWaveform(droneWaveforms[snapIndex][wfIdx], TABLE_SIZE);
                    waveformDroneB_5.frequency(freq);
                    waveformDroneB_5.amplitude(0.25);
                    ampDroneB_5.gain(amp);
                    SV_ladderDroneB_5.frequency(snap.cutoff);
                    SV_ladderDroneB_5.resonance(snap.resonance);
                    filterDroneBHP_5.frequency(snap.hpfFreq);
                    envelopeDroneB_Amplitude_5.attack(snap.envAmpAttack);
                    envelopeDroneB_Amplitude_5.decay(snap.envAmpDecay);
                    envelopeDroneB_Amplitude_5.sustain(snap.envAmpSustain);
                    envelopeDroneB_Amplitude_5.release(snap.envAmpRelease);
                    envelopeDroneB_Amplitude_5.noteOn();
                    break;
            }
        }

        // Synchronize per-snapshot envelope bookkeeping (start attack)
        if (useGroupA) {
            // start envelope timing for this snapshot
            droneEnvelopeStart[snapIndex] = millis();
            droneReleaseStart[snapIndex] = 0;
            droneFilter_Attack_Coeff[snapIndex] = 0.0f; // attack will ramp 0->1
            droneFilter_Decay_Coeff[snapIndex] = 1.0f;
            droneFilter_Sustain_Coeff[snapIndex] = 1.0f;
            droneFilter_Release_Coeff[snapIndex] = 1.0f;
            // prev flags
            dronePrevNoteOn[snapIndex] = true;
            dronePrevReleasing[snapIndex] = false;
            // init saved coefs
            drone_savedAttackCoef[snapIndex] = 1.0f;
            drone_savedDecayCoef[snapIndex] = 1.0f;
            drone_savedSustainCoef[snapIndex] = 1.0f;
        } else {
            droneEnvelopeStart[snapIndex] = millis();
            droneReleaseStart[snapIndex] = 0;
            droneFilter_Attack_Coeff[snapIndex] = 0.0f;
            droneFilter_Decay_Coeff[snapIndex] = 1.0f;
            droneFilter_Sustain_Coeff[snapIndex] = 1.0f;
            droneFilter_Release_Coeff[snapIndex] = 1.0f;
            dronePrevNoteOn[snapIndex] = true;
            dronePrevReleasing[snapIndex] = false;
            drone_savedAttackCoef[snapIndex] = 1.0f;
            drone_savedDecayCoef[snapIndex] = 1.0f;
            drone_savedSustainCoef[snapIndex] = 1.0f;
        }
 
    } // end for
  // Ensure per-snapshot envelope state is ready so DroneEnvelopes_update() sees the rising edge
  // Clear release/start bookkeeping for this snapshot (so attack starts fresh)
  if (snapIndex >= 0 && snapIndex < DRONE_SLOTS) {
      dronePrevNoteOn[snapIndex] = false;
      dronePrevReleasing[snapIndex] = false;
      droneReleaseStart[snapIndex] = 0;
      // Reset stored saved* so release logic later will capture the state correctly on noteOff
      drone_savedAttackCoef[snapIndex] = 1.0f;
      drone_savedDecayCoef[snapIndex]  = 1.0f;
      drone_savedSustainCoef[snapIndex]= 1.0f;
  }
}

// Stop (release) all voices of group A or B
static void stopGroup(bool useGroupA) {
    if (useGroupA) {
        envelopeDroneA_Amplitude_1.noteOff();
        envelopeDroneA_Amplitude_2.noteOff();
        envelopeDroneA_Amplitude_3.noteOff();
        envelopeDroneA_Amplitude_4.noteOff();
        envelopeDroneA_Amplitude_5.noteOff();
        envelopeNoiseDroneA.noteOff();

        // set group as releasing and save the releasing index
        if (activeDroneA >= 0) {
            releasingDroneA_index = activeDroneA;
            // start release timing for that snapshot
            unsigned long now = millis();
            droneReleaseStart[releasingDroneA_index] = now;
            // save current coefficients for smooth release
            drone_savedAttackCoef[releasingDroneA_index]  = droneFilter_Attack_Coeff[releasingDroneA_index];
            drone_savedDecayCoef[releasingDroneA_index]   = droneFilter_Decay_Coeff[releasingDroneA_index];
            drone_savedSustainCoef[releasingDroneA_index] = droneFilter_Sustain_Coeff[releasingDroneA_index];
        }
        droneGroupA_releasing = true;
    } else {
        envelopeDroneB_Amplitude_1.noteOff();
        envelopeDroneB_Amplitude_2.noteOff();
        envelopeDroneB_Amplitude_3.noteOff();
        envelopeDroneB_Amplitude_4.noteOff();
        envelopeDroneB_Amplitude_5.noteOff();
        envelopeNoiseDroneB.noteOff();

        if (activeDroneB >= 0) {
            releasingDroneB_index = activeDroneB;
            unsigned long now = millis();
            droneReleaseStart[releasingDroneB_index] = now;
            drone_savedAttackCoef[releasingDroneB_index]  = droneFilter_Attack_Coeff[releasingDroneB_index];
            drone_savedDecayCoef[releasingDroneB_index]   = droneFilter_Decay_Coeff[releasingDroneB_index];
            drone_savedSustainCoef[releasingDroneB_index] = droneFilter_Sustain_Coeff[releasingDroneB_index];
        }
        droneGroupB_releasing = true;
    }
}

// Called when a drone key is pressed
void DroneKeyOn(byte note) {
    int snapIndex = findSnapshotIndex(note);
    if (snapIndex < 0 || DRONEmodeSelect == 0) return; // not a drone key or mode off

    // Assign to drone group A or B
    if (activeDroneA < 0) {
        activeDroneA = snapIndex;
        playSnapshot(snapIndex, true);
    }
    else if (activeDroneB < 0) {
        activeDroneB = snapIndex;
        playSnapshot(snapIndex, false);
    }
    else {
        droneGroupActive ^= 1; // toggle 0 <-> 1 
        if (droneGroupActive == 0) {
            stopGroup(true); 
            activeDroneA = snapIndex; 
            playSnapshot(snapIndex, true); 
        }    
        else if (droneGroupActive == 1) {
            stopGroup(false); 
            activeDroneB = snapIndex; 
            playSnapshot(snapIndex, false);
        }    
    }
}

// Called when a drone key is released (in hold mode this may be ignored)
void DroneKeyOff(byte note) {
    if (DRONEmodeSelect == 0) return;
    int snapIndex = findSnapshotIndex(note);
    if (activeDroneA == snapIndex) {
        stopGroup(true);
    }
    if (activeDroneB == snapIndex) {
        stopGroup(false);
    }
}

// wrapper function: plays snapshot according to its slot index (0..7)
void DronePlaySlot(uint8_t slot) {
    if (slot >= 8) return;
    // if Save-mode == 1, ignore play — safety
    if (DRONEmodeSelect == 1) return;

    // same logic as in DroneKeyOn, but uses the slot directly
    if (activeDroneA < 0) {
        activeDroneA = slot;
        playSnapshot(slot, true);
        return;
    }
    if (activeDroneB < 0) {
        activeDroneB = slot;
        playSnapshot(slot, false);
        return;
    }
    // both used -> release and replace "the older one" (uses simple toggle to determine)
    droneGroupActive ^= 1; // toggle 0 <-> 1 
    if (droneGroupActive == 0) {
        stopGroup(true); 
        activeDroneA = slot; 
        playSnapshot(slot, true); 
    }    
    else if (droneGroupActive == 1) {
        stopGroup(false); 
        activeDroneB = slot; 
        playSnapshot(slot, false);
    }  
}

void DroneStopSlot(uint8_t slot) {
    if (slot >= 8) return;
    if (DRONEmodeSelect == 1) return; // in Save-mode ignore stop

    if (activeDroneA == (int)slot) {
        stopGroup(true);
    }
    if (activeDroneB == (int)slot) {
        stopGroup(false);
    }
}

// Drone LFO update (called with the same interval as the main LFO)
// calculates LFO for groups A/B and applies modulation on active drone voices
void DroneLFO_update() {
    DroneEnvelopes_update();
    if (activeDroneA < 0 && activeDroneB < 0 && !droneGroupA_releasing && !droneGroupB_releasing) return;

    const float updateInterval_s = LFO_UPDATE_INTERVAL / 1000.0f;

    auto lfoValueFromPhase = [&](float phase, int waveSelect) -> float {
        int idx = (int)(phase * LFO_TABLE_SIZE);
        if (idx < 0) idx = 0;
        if (idx >= LFO_TABLE_SIZE) idx = LFO_TABLE_SIZE - 1;
        const int16_t* tbl = LFOTables[(waveSelect >= 0 && waveSelect < 5) ? waveSelect : 0];
        int16_t raw = tbl[idx];
        return (raw + 32768.0f) / 65535.0f;
    };

    auto processGroup = [&](bool isA) {
        unsigned long LFOrate_us = isA ? DroneA_LFOrate_us : DroneB_LFOrate_us;
        float &LFOdepth = isA ? DroneA_LFOdepth : DroneB_LFOdepth;
        int &LFOwaveSelect = isA ? DroneA_LFOwaveSelect : DroneB_LFOwaveSelect;
        byte &LFOmodeSelect = isA ? DroneA_LFOmodeSelect : DroneB_LFOmodeSelect;
        byte &LFOtypeSelect = isA ? DroneA_LFOtypeSelect : DroneB_LFOtypeSelect;
        unsigned long &LFOdelayEndMillis = isA ? DroneA_LFOdelayEndMillis : DroneB_LFOdelayEndMillis;
        unsigned long &LFOdelay_ms = isA ? DroneA_LFOdelay_ms : DroneB_LFOdelay_ms;
        float &LFOphase = isA ? DroneA_LFOphase : DroneB_LFOphase;
        float &LFOonce  = isA ? DroneA_LFOonce : DroneB_LFOonce;
        bool  &LFOactive = isA ? DroneA_LFOactive : DroneB_LFOactive;
        bool  &LFOstop   = isA ? DroneA_LFOstop : DroneB_LFOstop;
        float &lfoOut    = isA ? DroneA_lfoOut : DroneB_lfoOut;

        (void)LFOdelay_ms; // suppress "unused" warning during compilation

        float detuneMultiplier = 1.0f;
        float cutoffMod = 1.0f;

        float safeRate_us = max(1.0f, (float)LFOrate_us);
        float lfoPeriod_s = safeRate_us / 1e6f;
        float phaseIncrement = updateInterval_s / lfoPeriod_s;
        float onceIncrement = phaseIncrement;

        bool delayed = false;
        if (LFOdelayEndMillis > 0 && millis() < LFOdelayEndMillis) {
            delayed = true;
            LFOactive = false;
        } else if (LFOdelayEndMillis > 0) {
            // delay just ended
            LFOdelayEndMillis = 0;
            if (LFOmodeSelect == 2) { 
              LFOonce = 0.0f; 
              LFOstop = false; 
              LFOactive = true; 
            }
            else if (LFOmodeSelect == 3) { 
              LFOonce = 1.0f; 
              LFOstop = false; 
              LFOactive = true; 
            }
            else { 
              LFOphase = 0.0f; 
              LFOactive = true; 
              LFOstop = false; 
            }
        }

        if (!delayed) {
            switch (LFOmodeSelect) {
                case 0: // FREE
                    if (!LFOstop) {
                        LFOphase += phaseIncrement;
                        if (LFOphase >= 1.0f) LFOphase -= (int)LFOphase;
                        LFOactive = true;
                    }
                    break;
                case 1: // RETRIG
                    if (!LFOstop) {
                        LFOphase += phaseIncrement;
                        if (LFOphase >= 1.0f) LFOphase -= (int)LFOphase;
                    }
                    break;
                case 2: // RISE oneshot
                    if (!LFOstop) {
                        LFOonce += onceIncrement;
                        if (LFOonce >= 1.0f) { 
                          LFOonce = 1.0f; 
                          LFOstop = true; 
                          LFOactive = false; 
                        }
                    }
                    break;
                case 3: // FALL oneshot
                    if (!LFOstop) {
                        LFOonce -= onceIncrement;
                        if (LFOonce <= 0.0f) { 
                          LFOonce = 0.0f; 
                          LFOstop = true; 
                          LFOactive = false; 
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        if (LFOmodeSelect == 2 || LFOmodeSelect == 3) {
            lfoOut = constrain(LFOonce, 0.0f, 1.0f);
        } else {
            lfoOut = lfoValueFromPhase(LFOphase, LFOwaveSelect);
        }

        // mapping of LFO modulation types
        // main LFO: 0 = freq/cutoff, 1 = shape, 2 = detune
        if (LFOtypeSelect == 0) {
            // CUTOFF / freq modulation: multiplicative factor similar to main LFO
            float modRange = 2.0f; // ±100% change
            float mod = 1.0f + modRange * LFOdepth * (lfoOut - 0.5f);
            // NaN/Inf protection
            if (!isfinite(mod)) mod = 1.0f;
            // safety clamp like in main LFO — prevents clipping
            mod = constrain(mod, 0.001f, 1.001f);
            cutoffMod = mod;
        }
        else if (LFOtypeSelect == 1) {
            // SHAPE modulation: produce blend factor 0..1 (we'll blend between saved waveforms)
            float blend = LFOdepth * lfoOut; // 0..1 scaled by depth
            blend = constrain(blend, 0.0f, 1.0f);
        }
        else if (LFOtypeSelect == 2) {
            // DETUNE / FREQ modulation: compute single multiplier for group
            float semitoneRange = 0.5f; // ±0.5 semitone default
            float semitones = semitoneRange * LFOdepth * (lfoOut - 0.5f) * 2.0f;
            float detMul = powf(2.0f, semitones / 12.0f);
            if (!isfinite(detMul) || detMul <= 0.0001f) detMul = 1.0f;
            detuneMultiplier = constrain(detMul, 0.5f, 2.0f);
        }

        // apply to active snapshot group
        int activeIndex = isA ? activeDroneA : activeDroneB;
        if (activeIndex < 0 || activeIndex >= DRONE_SLOTS) return;
        DroneSnapshot &snap = droneSnapshots[activeIndex];
        if (!snap.valid) return;

        // compute per-snapshot envelope coefficient once (applies regardless of LFO type)
        float envCoeff = 1.0f;
        if (activeIndex >= 0 && activeIndex < DRONE_SLOTS) {
            envCoeff = droneFilterEnvelope_Coeff[activeIndex];
            // defensive clamp
            if (!isfinite(envCoeff)) envCoeff = 1.0f;
            if (envCoeff < 0.0f) envCoeff = 0.0f;
            if (envCoeff > 1.0f) envCoeff = 1.0f;
        }

        // precompute base cutoff already modulated by envelope (LFO cutoff multiplier applied per-voice later)
        float baseCutByEnv = snap.cutoff * envCoeff;

        // iterate voices and apply LFO/type effects; ALWAYS write the SV_ladder frequency using the
        // envelope-modulated value (and then apply additional LFO cutoff multiplier when requested).
        for (int i = 0; i < 5; ++i) {
            bool activeHere = snap.voiceActive[i];
            if (SynthMode == 0) { 
              if (i == 3) activeHere = snap.voiceActive[0]; 
              if (i == 4) activeHere = false; 
            }
            if (!activeHere) continue;

            int noteIndex;
            if (SynthMode == 0) {
                if (i <= 2) noteIndex = (int)snap.note[i];
                else if (i == 3) noteIndex = (int)snap.note[0] + snap.savedSubOctave;
                else noteIndex = (int)snap.note[i];
            } else noteIndex = (int)snap.note[i];
            noteIndex = constrain(noteIndex, 0, 127);
            float baseFreq = noteFreqs[noteIndex];

            int wfIdx = (i < DRONE_WAVE_VOICES) ? i : (DRONE_WAVE_VOICES - 1);
            // --- LFO TYPE HANDLING: detune / cutoff / shape / fallback ---
            // ALWAYS ensure cutoff gets updated by envelope (baseCutByEnv)
            if (LFOtypeSelect == 2) {
                // DETUNE / FREQ modulation: compute single multiplier for group and apply to oscillator freq
                float newFreq = baseFreq * detuneMultiplier;
                if (isA) {
                    switch (i) {
                        case 0: waveformDroneA_1.frequency(newFreq); break;
                        case 1: waveformDroneA_2.frequency(newFreq); break;
                        case 2: waveformDroneA_3.frequency(newFreq); break;
                        case 3: waveformDroneA_4.frequency(newFreq); break;
                        case 4: waveformDroneA_5.frequency(newFreq); break;
                    }
                } else {
                    switch (i) {
                        case 0: waveformDroneB_1.frequency(newFreq); break;
                        case 1: waveformDroneB_2.frequency(newFreq); break;
                        case 2: waveformDroneB_3.frequency(newFreq); break;
                        case 3: waveformDroneB_4.frequency(newFreq); break;
                        case 4: waveformDroneB_5.frequency(newFreq); break;
                    }
                }
                // also keep cutoff updated by envelope even when detuning
                float finalCut_noLFO_detune = baseCutByEnv;
                if (isA) {
                    switch (i) {
                        case 0: SV_ladderDroneA_1.frequency(finalCut_noLFO_detune); break;
                        case 1: SV_ladderDroneA_2.frequency(finalCut_noLFO_detune); break;
                        case 2: SV_ladderDroneA_3.frequency(finalCut_noLFO_detune); break;
                        case 3: SV_ladderDroneA_4.frequency(finalCut_noLFO_detune); break;
                        case 4: SV_ladderDroneA_5.frequency(finalCut_noLFO_detune); break;
                    }
                } else {
                    switch (i) {
                        case 0: SV_ladderDroneB_1.frequency(finalCut_noLFO_detune); break;
                        case 1: SV_ladderDroneB_2.frequency(finalCut_noLFO_detune); break;
                        case 2: SV_ladderDroneB_3.frequency(finalCut_noLFO_detune); break;
                        case 3: SV_ladderDroneB_4.frequency(finalCut_noLFO_detune); break;
                        case 4: SV_ladderDroneB_5.frequency(finalCut_noLFO_detune); break;
                    }
                }
            }
            else if (LFOtypeSelect == 0) {
                // CUTOFF / freq modulation: apply envelope first, then LFO multiplier
                float newCut = baseCutByEnv * cutoffMod;
                if (isA) {
                    switch (i) {
                        case 0: SV_ladderDroneA_1.frequency(newCut); break;
                        case 1: SV_ladderDroneA_2.frequency(newCut); break;
                        case 2: SV_ladderDroneA_3.frequency(newCut); break;
                        case 3: SV_ladderDroneA_4.frequency(newCut); break;
                        case 4: SV_ladderDroneA_5.frequency(newCut); break;
                    }
                } else {
                    switch (i) {
                        case 0: SV_ladderDroneB_1.frequency(newCut); break;
                        case 1: SV_ladderDroneB_2.frequency(newCut); break;
                        case 2: SV_ladderDroneB_3.frequency(newCut); break;
                        case 3: SV_ladderDroneB_4.frequency(newCut); break;
                        case 4: SV_ladderDroneB_5.frequency(newCut); break;
                    }
                }
            }
            else if (LFOtypeSelect == 1) {
                // SHAPE modulation: perform morphing THEN ensure cutoff is set by envelope (no LFO multiplier)
                bool voiceActiveHere = snap.voiceActive[i];
                if (SynthMode == 0) { 
                  if (i == 3) voiceActiveHere = snap.voiceActive[0]; 
                  if (i == 4) voiceActiveHere = false; 
                }
                if (voiceActiveHere) {
                    uint8_t savedWfIdx = 0;
                    int savedShapeIdx = 0;
                    if (wfIdx >= 0 && wfIdx < DRONE_WAVE_VOICES) {
                        savedWfIdx    = snap.savedWaveFamilyIndex[wfIdx];
                        savedShapeIdx = snap.savedShapeIndex[wfIdx];
                    }

                    int familyIndex = (int)savedWfIdx;
                    if (familyIndex < 0) familyIndex = 0;
                    const int nFamilies = NUM_WAVEFAMILIES;
                    const WaveFamily& wf = waveFamilies[ familyIndex % nFamilies ];

                    int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                    int morphSteps = max(1, wf.morphSteps);

                    float offset = -LFOdepth * lfoOut * (float)maxShapeIndex;
                    if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                        offset = -offset;
                    }

                    int modulatedShapeIndex;
                    if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                        modulatedShapeIndex = constrain((int)((maxShapeIndex - savedShapeIdx) + offset), 0, maxShapeIndex);
                    } else {
                        modulatedShapeIndex = constrain((int)(savedShapeIdx + offset), 0, maxShapeIndex);
                    }

                    int baseIndex = (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
                    int step = (morphSteps > 0) ? (modulatedShapeIndex % morphSteps) : 0;
                    float t = (morphSteps > 0) ? (step / (float)morphSteps) : 0.0f;

                    const int maxWaveIdx = max(0, wf.numWaves - 1);
                    const int16_t* waveA = wf.waves[ baseIndex <= maxWaveIdx ? baseIndex : maxWaveIdx ];
                    const int16_t* waveB = wf.waves[ min(baseIndex + 1, maxWaveIdx) ];

                    int16_t* dest = droneWaveforms[activeIndex][wfIdx];
                    for (int s = 0; s < TABLE_SIZE; ++s) {
                        float fa = (float)waveA[s];
                        float fb = (float)waveB[s];
                        dest[s] = (int16_t)((1.0f - t) * fa + t * fb);
                    }

                    if (isA) {
                        switch (i) {
                            case 0: waveformDroneA_1.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 1: waveformDroneA_2.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 2: waveformDroneA_3.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 3: waveformDroneA_4.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 4: waveformDroneA_5.arbitraryWaveform(dest, TABLE_SIZE); break;
                        }
                    } else {
                        switch (i) {
                            case 0: waveformDroneB_1.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 1: waveformDroneB_2.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 2: waveformDroneB_3.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 3: waveformDroneB_4.arbitraryWaveform(dest, TABLE_SIZE); break;
                            case 4: waveformDroneB_5.arbitraryWaveform(dest, TABLE_SIZE); break;
                        }
                    }
                }
                // After shape morphing, still set cutoff by envelope
                {
                    float finalCut_noLFO_shape = baseCutByEnv;
                    if (isA) {
                        switch (i) {
                            case 0: SV_ladderDroneA_1.frequency(finalCut_noLFO_shape); break;
                            case 1: SV_ladderDroneA_2.frequency(finalCut_noLFO_shape); break;
                            case 2: SV_ladderDroneA_3.frequency(finalCut_noLFO_shape); break;
                            case 3: SV_ladderDroneA_4.frequency(finalCut_noLFO_shape); break;
                            case 4: SV_ladderDroneA_5.frequency(finalCut_noLFO_shape); break;
                        }
                    } else {
                        switch (i) {
                            case 0: SV_ladderDroneB_1.frequency(finalCut_noLFO_shape); break;
                            case 1: SV_ladderDroneB_2.frequency(finalCut_noLFO_shape); break;
                            case 2: SV_ladderDroneB_3.frequency(finalCut_noLFO_shape); break;
                            case 3: SV_ladderDroneB_4.frequency(finalCut_noLFO_shape); break;
                            case 4: SV_ladderDroneB_5.frequency(finalCut_noLFO_shape); break;
                        }
                    }
                }
            }
            else {
                // fallback: no special LFO type — ensure cutoff honors envelope
                float finalCut_noLFO = baseCutByEnv;
                if (isA) {
                    switch (i) {
                        case 0: SV_ladderDroneA_1.frequency(finalCut_noLFO); break;
                        case 1: SV_ladderDroneA_2.frequency(finalCut_noLFO); break;
                        case 2: SV_ladderDroneA_3.frequency(finalCut_noLFO); break;
                        case 3: SV_ladderDroneA_4.frequency(finalCut_noLFO); break;
                        case 4: SV_ladderDroneA_5.frequency(finalCut_noLFO); break;
                    }
                } else {
                    switch (i) {
                        case 0: SV_ladderDroneB_1.frequency(finalCut_noLFO); break;
                        case 1: SV_ladderDroneB_2.frequency(finalCut_noLFO); break;
                        case 2: SV_ladderDroneB_3.frequency(finalCut_noLFO); break;
                        case 3: SV_ladderDroneB_4.frequency(finalCut_noLFO); break;
                        case 4: SV_ladderDroneB_5.frequency(finalCut_noLFO); break;
                    }
                }        
            } // end shape branch
        }
    };

// finalize: if waiting release, check that amplitude envelope is already off
auto finalizeGroupIfDone = [&]() {
    // GROUP A
    if (droneGroupA_releasing && releasingDroneA_index >= 0) {
        bool anyActive =
            envelopeDroneA_Amplitude_1.isActive() ||
            envelopeDroneA_Amplitude_2.isActive() ||
            envelopeDroneA_Amplitude_3.isActive() ||
            envelopeDroneA_Amplitude_4.isActive() ||
            envelopeDroneA_Amplitude_5.isActive();

        if (!anyActive) {
            // mute oscillators + gains
            waveformDroneA_1.amplitude(0); 
            waveformDroneA_2.amplitude(0); 
            waveformDroneA_3.amplitude(0);
            waveformDroneA_4.amplitude(0); 
            waveformDroneA_5.amplitude(0);
            ampDroneA_1.gain(0); 
            ampDroneA_2.gain(0); 
            ampDroneA_3.gain(0); 
            ampDroneA_4.gain(0); 
            ampDroneA_5.gain(0);

            // release activeDroneA if it is the active slot
            if (activeDroneA == releasingDroneA_index) {
                activeDroneA = -1;
            }
            // reset release bookkeeping
            droneGroupA_releasing = false;
            releasingDroneA_index = -1;
        }
    }

    // GROUP B
    if (droneGroupB_releasing && releasingDroneB_index >= 0) {
        bool anyActive =
            envelopeDroneB_Amplitude_1.isActive() ||
            envelopeDroneB_Amplitude_2.isActive() ||
            envelopeDroneB_Amplitude_3.isActive() ||
            envelopeDroneB_Amplitude_4.isActive() ||
            envelopeDroneB_Amplitude_5.isActive();

        if (!anyActive) {
            waveformDroneB_1.amplitude(0); 
            waveformDroneB_2.amplitude(0); 
            waveformDroneB_3.amplitude(0);
            waveformDroneB_4.amplitude(0); 
            waveformDroneB_5.amplitude(0);
            ampDroneB_1.gain(0); 
            ampDroneB_2.gain(0); 
            ampDroneB_3.gain(0); 
            ampDroneB_4.gain(0); 
            ampDroneB_5.gain(0);

            if (activeDroneB == releasingDroneB_index) {
                activeDroneB = -1;
            }
            droneGroupB_releasing = false;
            releasingDroneB_index = -1;
        }
    }
};  // lambda has to be closed with semicolon

// calculate/apply LFO, so that it modulates even when releasing
processGroup(true);
processGroup(false);

// only then end releasing groups
finalizeGroupIfDone();
}

// Update per-snapshot drone filter envelopes
void DroneEnvelopes_update() {
    unsigned long now = millis();

    for (int s = 0; s < DRONE_SLOTS; ++s) {
        DroneSnapshot &snap = droneSnapshots[s];

        // if snapshot invalid -> reset and skip
        if (!snap.valid) {
            droneFilterEnvelope_Coeff[s] = 1.0f;
            dronePrevNoteOn[s] = false;
            dronePrevReleasing[s] = false;
            continue;
        }

        // Determine whether this snapshot is currently sounding or releasing
        // rawNoteOn = assigned to A or B; releasing = flagged by stopGroup bookkeeping
        bool rawNoteOn = (activeDroneA == s) || (activeDroneB == s);
        bool releasing = (droneGroupA_releasing && releasingDroneA_index == s) ||
                         (droneGroupB_releasing && releasingDroneB_index == s);
        // Treat snapshot as "noteOn" only if it's assigned AND not currently in releasing state.
        // This prevents immediate cancellation of the release branch when activeDroneX is still set.
        bool noteOn = rawNoteOn && !releasing;

        // Rising edge: note on
        if (!dronePrevNoteOn[s] && noteOn) {
            droneEnvelopeStart[s] = now;
            droneReleaseStart[s] = 0;
            droneFilter_Attack_Coeff[s] = 0.0f;
            droneFilter_Decay_Coeff[s] = 1.0f;
            droneFilter_Sustain_Coeff[s] = 1.0f;
            droneFilter_Release_Coeff[s] = 1.0f;
        }

        // Rising edge: release started
        if (releasing && !dronePrevReleasing[s]) {
            // save current state and set releaseStart (stopGroup() already set releaseStart, but set it here too)
            if (droneReleaseStart[s] == 0) droneReleaseStart[s] = now;
            drone_savedAttackCoef[s]  = droneFilter_Attack_Coeff[s];
            drone_savedDecayCoef[s]   = droneFilter_Decay_Coeff[s];
            drone_savedSustainCoef[s] = droneFilter_Sustain_Coeff[s];
        }

        // Use snapshot's ADSR values (snap.envFilter*)
        float attackMs  = snap.envFilterAttack;   // ms
        float decayMs   = snap.envFilterDecay;    // ms
        float sustainV  = snap.envFilterSustain;  // 0..1
        float releaseMs = snap.envFilterRelease;  // ms

        // Compute coefficients
        if (noteOn) {
            // cancel release
            droneReleaseStart[s] = 0;
            float totalElapsed = (float)(now - droneEnvelopeStart[s]); // ms

            // ATTACK 0 -> 1
            if (attackMs > 0.0f && totalElapsed < attackMs) {
                float t = totalElapsed / attackMs;
                droneFilter_Attack_Coeff[s] = t;
            } else {
                droneFilter_Attack_Coeff[s] = 1.0f;
            }

            // DECAY 1 -> sustain
            if (decayMs > 0.0f && totalElapsed > attackMs && totalElapsed < (attackMs + decayMs)) {
                float decayElapsed = totalElapsed - attackMs;
                float t = decayElapsed / decayMs;
                droneFilter_Decay_Coeff[s] = 1.0f - t * (1.0f - sustainV);
            } else {
                droneFilter_Decay_Coeff[s] = 1.0f;
            }

            // SUSTAIN
            if ((decayMs > 0.0f && totalElapsed >= (attackMs + decayMs)) ||
                (decayMs <= 0.0f && totalElapsed >= attackMs)) {
                droneFilter_Sustain_Coeff[s] = sustainV;
            } else {
                droneFilter_Sustain_Coeff[s] = 1.0f;
            }

            droneFilter_Release_Coeff[s] = 1.0f; // neutral while note held
        } else {
            // not noteOn -> compute release if active
            if (droneReleaseStart[s] == 0) {
                // no release active -> neutral
                droneFilter_Attack_Coeff[s] = 1.0f;
                droneFilter_Decay_Coeff[s] = 1.0f;
                droneFilter_Sustain_Coeff[s] = 1.0f;
            } else {
                // release running
                float relElapsed = (float)(now - droneReleaseStart[s]); // ms
                if (releaseMs > 0.0f) {
                    if (relElapsed < releaseMs) {
                        float t = relElapsed / releaseMs;
                        droneFilter_Release_Coeff[s] = 1.0f - t; // 1->0
                    } else {
                        // release TIME finished -> set final release value (0.0)
                        droneFilter_Release_Coeff[s] = 0.0f;
                    }
                } else {
                    // instant release
                    droneFilter_Release_Coeff[s] = 1.0f;
                    droneReleaseStart[s] = 0;
                    drone_savedAttackCoef[s]  = 1.0f;
                    drone_savedDecayCoef[s]   = 1.0f;
                    drone_savedSustainCoef[s] = 1.0f;
                    droneFilter_Attack_Coeff[s] = 1.0f;
                    droneFilter_Decay_Coeff[s]  = 1.0f;
                    droneFilter_Sustain_Coeff[s] = 1.0f;
                }

                // while release is active, hold saved attack/decay/sustain to allow smooth fall
                if (droneReleaseStart[s] != 0) {
                    droneFilter_Attack_Coeff[s]  = drone_savedAttackCoef[s];
                    droneFilter_Decay_Coeff[s]   = drone_savedDecayCoef[s];
                    droneFilter_Sustain_Coeff[s] = drone_savedSustainCoef[s];
                }
            }
        }

        // Final product (multiplicative)
        float baseCoeff =
            droneFilter_Attack_Coeff[s] *
            droneFilter_Decay_Coeff[s] *
            droneFilter_Sustain_Coeff[s] *
            droneFilter_Release_Coeff[s];

        // INTENSITY: use per-snapshot saved intensity (expected -1..1)
        float F = snap.envFilterIntensity;

        // clamp to -1..1 just in case
        if (F < -1.0f) F = -1.0f;
        if (F >  1.0f) F =  1.0f;

        // magnitude 0..1
        float m = fabsf(F);

        // target: normal baseCoeff (F>=0) or inverted (F<0)
        float target = (F >= 0.0f) ? baseCoeff : (1.0f - baseCoeff);

        // interpolate from neutral (1.0) toward target by amount m
        // m=0 -> 1.0 (no effect), m=1 -> target (full effect)
        float newFilterEnv = 1.0f - m * (1.0f - target);

        // store per-slot drone coefficient
        droneFilterEnvelope_Coeff[s] = newFilterEnv;

        // RELEASE HANDLING: follow same semantics as main engine
        if (droneReleaseStart[s] != 0 && !releasing) {
            // fully reset only when the snapshot is no longer releasing
            droneReleaseStart[s] = 0;
            drone_savedAttackCoef[s]  = 1.0f;
            drone_savedDecayCoef[s]   = 1.0f;
            drone_savedSustainCoef[s] = 1.0f;
            droneFilter_Attack_Coeff[s]   = 1.0f;
            droneFilter_Decay_Coeff[s]    = 1.0f;
            droneFilter_Sustain_Coeff[s]  = 1.0f;
            droneFilter_Release_Coeff[s]  = 1.0f;
            droneFilterEnvelope_Coeff[s]  = 1.0f;
        }

        // Update prev flags
        dronePrevNoteOn[s] = noteOn;
        dronePrevReleasing[s] = releasing;
    } // end for
}
// ==== end of DRONE ====