// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Drone.h"
#include "Synthesis\ShapeDetuneRecomputeFix.h"
// ==================

void ShapeDetuneRecomputeFix() { // Fix function that initializes Shape/Detune LFO - without this, the audio path stops
// --- FIX 1: recompute shape index pro stabilní chování LFO ---
    if (LFOtypeSelect == 1 && (voiceActive_1 || voiceReleasing_1)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_1];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_1 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_1) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_1 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

    // --- FIX 2 ---
    if (LFOtypeSelect == 1 && (voiceActive_2 || voiceReleasing_2)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_2];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_2* maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_2) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_2 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

    // --- FIX 3 ---
    if (LFOtypeSelect == 1 && (voiceActive_3 || voiceReleasing_3)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_3 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_3 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

    // --- FIX 4: voice 4 UNISON ---
    if ((SynthMode == 0) && (LFOtypeSelect == 1) && (voiceActive_4 || voiceReleasing_4)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_1];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_4 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_1) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_1 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

        // --- FIX 5: voice 5 UNISON ---
    if ((SynthMode == 0) && (LFOtypeSelect == 1) && (voiceActive_5 || voiceReleasing_5)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_2];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_5 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_2) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_2 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

        // --- FIX 6: voice 6 UNISON ---
    if ((SynthMode == 0) && (LFOtypeSelect == 1) && (voiceActive_6 || voiceReleasing_6)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_6 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_3 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

    // --- FIX 7: voice 4 POLY ---
    if ((SynthMode == 1) && (LFOtypeSelect == 1) && (voiceActive_4 || voiceReleasing_4)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_4 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_3 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

        // --- FIX 8: voice 5 POLY ---
    if ((SynthMode == 1) && (LFOtypeSelect == 1) && (voiceActive_5 || voiceReleasing_5)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_5 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_3 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }

        // --- FIX 9: voice 6 POLY ---
    if ((SynthMode == 1) && (LFOtypeSelect == 1) && (voiceActive_6 || voiceReleasing_6)) {
        const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
        int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
        int morphSteps = wf.morphSteps;

        float offset = -LFOdepth * lfoOut_6 * maxShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1)
            offset = -offset;

        __attribute__((unused)) int modulatedShapeIndex;
        if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
            modulatedShapeIndex = constrain(
                (int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex
            );
        } else {
            modulatedShapeIndex = constrain(
                (int)(currentShapeIndex_3 + offset), 0, maxShapeIndex
            );
        }

        __attribute__((unused)) int baseIndex =
            (morphSteps > 0) ? (modulatedShapeIndex / morphSteps) : 0;
    }
}