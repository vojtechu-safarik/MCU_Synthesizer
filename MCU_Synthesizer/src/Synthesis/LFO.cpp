// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\LFO.h"
// ==================

void resetLFOstate() {
    // === Voice 1 ===
    if (voiceActive_1) {
        LFOphase_1  = 0.0f;
        LFOonce_1   = (LFOmodeSelect == 3) ? 1.0f : 0.0f; // FALL starts from up and goes down, duh
        LFOstop_1   = false;
        LFOactive_1 = true;
        lastGate_1  = gate_1;
    }

    // === Voice 2 ===
    if (voiceActive_2) {
        LFOphase_2  = 0.0f;
        LFOonce_2   = (LFOmodeSelect == 3) ? 1.0f : 0.0f;
        LFOstop_2   = false;
        LFOactive_2 = true;
        lastGate_2  = gate_2;
    }

    // === Voice 3 ===
    if (voiceActive_3) {
        LFOphase_3  = 0.0f;
        LFOonce_3   = (LFOmodeSelect == 3) ? 1.0f : 0.0f;
        LFOstop_3   = false;
        LFOactive_3 = true;
        lastGate_3  = gate_3;
    }

    // === Voice 4 ===
    if (voiceActive_4) {
        LFOphase_4  = 0.0f;
        LFOonce_4   = (LFOmodeSelect == 3) ? 1.0f : 0.0f;
        LFOstop_4   = false;
        LFOactive_4 = true;
        lastGate_4  = gate_4;
    }

        // === Voice 5 ===
    if (voiceActive_5) {
        LFOphase_5  = 0.0f;
        LFOonce_5   = (LFOmodeSelect == 3) ? 1.0f : 0.0f;
        LFOstop_5   = false;
        LFOactive_5 = true;
        lastGate_5  = gate_5;
    }

    // === Voice 6 ===
    if (voiceActive_6) {
        LFOphase_6  = 0.0f;
        LFOonce_6   = (LFOmodeSelect == 3) ? 1.0f : 0.0f;
        LFOstop_6   = false;
        LFOactive_6 = true;
        lastGate_6  = gate_6;
    }
}