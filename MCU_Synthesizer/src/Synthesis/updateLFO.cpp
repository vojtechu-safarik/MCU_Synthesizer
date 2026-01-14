// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
#include "Global_Variables\Wavetable_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\updateMorphWaveform.h"
#include "Synthesis\LFO.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\resetAllVoices.h"
// ==================

// === DISPLAY FUNCTIONS ===
#include "Display\Display.h"
// ==================

void LFOupdate() {
    // === PHASE INCREMENT ACCORDING TO LFOrate ===
    // LFOrate in µs – length of one period
    float safeRate_us = max(1.0f, (float)LFOrate);  // at minimum 10 ms = 0.1 Hz
    float lfoPeriod_s = safeRate_us / 1e6f;
    float updateInterval_s = LFO_UPDATE_INTERVAL / 1000.0f;

    float phaseIncrement = updateInterval_s / lfoPeriod_s;
    float LFOonceIncrement = updateInterval_s / lfoPeriod_s;  // full  range is 0 to 1

    // === Voice 1 ===
    if (voiceActive_1 || LFOactive_1 || voiceReleasing_1) {
        // delay check (per-voice)
        bool stillDelaying_1 = false;
        if (LFOdelaying_1) {
            if ((unsigned long)lfoTimer >= LFOdelayEnd_1) {
                LFOdelaying_1 = false;
            } else {
                stillDelaying_1 = true;
            }
        }

        // Detect one‐shot retrig (noteOn from MyNoteOn sets gate_1 = true)
        bool oneShotRetrig_1 = (LFOmodeSelect == 2 || LFOmodeSelect == 3) && gate_1 && !stillDelaying_1;
        if (oneShotRetrig_1) {
            // reset one‐shot before the first incrementation
            LFOonce_1 = (LFOmodeSelect == 2) ? 0.0f : 1.0f;
            LFOstop_1 = false;
            LFOactive_1 = true; // oneshot runs even if note ends
        }

        // === Detect leading edge of gate signal ===
        bool retrig_1 = false;
        switch (LFOmodeSelect) {
            case 0: // FREE mode
                retrig_1 = false;
            
                if (SynthMode == 0) {
                  if (!stillDelaying_1) {
                    if (unisonTriad == 0) LFOglobalPhase += phaseIncrement;
                    if (LFOglobalPhase >= 1.0f) LFOglobalPhase -= 1.0f;
                  }
                } else if (SynthMode == 1) {
                  if (!stillDelaying_1) {
                    LFOphase_1 += phaseIncrement;
                    if (LFOphase_1 >= 1.0f) LFOphase_1 -= 1.0f;
                  }
                }
                break;

            case 1: // RETRIG mode
                retrig_1 = (!lastGate_1 && gate_1) && !stillDelaying_1;
                if (retrig_1) LFOphase_1 = 0.0f;
                if (!stillDelaying_1) {
                    LFOphase_1 += phaseIncrement;
                    if (LFOphase_1 >= 1.0f) LFOphase_1 -= 1.0f;
                }
                lastGate_1 = gate_1;
                break;

            case 2: // RISE mode
                if (!stillDelaying_1 && !LFOstop_1) {
                    LFOonce_1 += LFOonceIncrement;
                    if (LFOonce_1 >= 1.0f) {
                        LFOonce_1 = 1.0f;
                        LFOstop_1 = true;
                        LFOactive_1 = false; // one-shot is over
                    }
                }
                lfoOut_1 = LFOonce_1;
                break;

            case 3: // FALL mode
                if (!stillDelaying_1 && !LFOstop_1) {
                    LFOonce_1 -= LFOonceIncrement;
                    if (LFOonce_1 <= 0.0f) {
                        LFOonce_1 = 0.0f;
                        LFOstop_1 = true;
                        LFOactive_1 = false; // one-shot is over
                    }
                }
                lfoOut_1 = LFOonce_1;
                break;

            default: // fallback - FREE mode
                retrig_1 = false;
                break;
        }
    }

    // === Voice 2 ===
    if (voiceActive_2 || LFOactive_2 || voiceReleasing_2) {
        // delay check (per-voice)
        bool stillDelaying_2 = false;
        if (LFOdelaying_2) {
            if ((unsigned long)lfoTimer >= LFOdelayEnd_2) {
                LFOdelaying_2 = false;
            } else {
                stillDelaying_2 = true;
            }
        }

        bool oneShotRetrig_2 = (LFOmodeSelect == 2 || LFOmodeSelect == 3) && gate_2 && !stillDelaying_2;
        if (oneShotRetrig_2) {
            LFOonce_2 = (LFOmodeSelect == 2) ? 0.0f : 1.0f;
            LFOstop_2 = false;
            LFOactive_2 = true;
        }

        bool retrig_2 = false;
        switch (LFOmodeSelect) {
            case 0:
                retrig_2 = false;
                if (SynthMode == 0) {
                  if (!stillDelaying_2) {
                    if (LFOglobalPhase >= 1.0f) LFOglobalPhase -= 1.0f;
                  }
                } else if (SynthMode == 1) {
                  if (!stillDelaying_2) {
                    if (LFOphase_1 >= 1.0f) LFOphase_1 -= 1.0f;
                  }
                }
                break;

            case 1:
                retrig_2 = (!lastGate_2 && gate_2) && !stillDelaying_2;
                if (retrig_2) LFOphase_2 = 0.0f;
                if (!stillDelaying_2) {
                    LFOphase_2 += phaseIncrement;
                    if (LFOphase_2 >= 1.0f) LFOphase_2 -= 1.0f;
                }
                lastGate_2 = gate_2;
                break;

            case 2: // RISE
                if (!stillDelaying_2 && !LFOstop_2) {
                    LFOonce_2 += LFOonceIncrement;
                    if (LFOonce_2 >= 1.0f) {
                        LFOonce_2 = 1.0f;
                        LFOstop_2 = true;
                        LFOactive_2 = false;
                    }
                }
                lfoOut_2 = LFOonce_2;
                break;

            case 3: // FALL
                if (!stillDelaying_2 && !LFOstop_2) {
                    LFOonce_2 -= LFOonceIncrement;
                    if (LFOonce_2 <= 0.0f) {
                        LFOonce_2 = 0.0f;
                        LFOstop_2 = true;
                        LFOactive_2 = false;
                    }
                }
                lfoOut_2 = LFOonce_2;
                break;

            default:
                retrig_2 = false;
                break;
        }
    }

    // === Voice 3 ===
    if (voiceActive_3 || LFOactive_3 || voiceReleasing_3) {
        bool stillDelaying_3 = false;
        if (LFOdelaying_3) {
            if ((unsigned long)lfoTimer >= LFOdelayEnd_3) {
                LFOdelaying_3 = false;
            } else {
                stillDelaying_3 = true;
            }
        }

        bool oneShotRetrig_3 = (LFOmodeSelect == 2 || LFOmodeSelect == 3) && gate_3 && !stillDelaying_3;
        if (oneShotRetrig_3) {
            LFOonce_3 = (LFOmodeSelect == 2) ? 0.0f : 1.0f;
            LFOstop_3 = false;
            LFOactive_3 = true;
        }

        bool retrig_3 = false;
        switch (LFOmodeSelect) {
            case 0:
                retrig_3 = false;
                if (SynthMode == 0) {
                  if (!stillDelaying_3) {
                    if (LFOglobalPhase >= 1.0f) LFOglobalPhase -= 1.0f;
                  }
                } else if (SynthMode == 1) {
                  if (!stillDelaying_3) {
                    if (LFOphase_1 >= 1.0f) LFOphase_1 -= 1.0f;
                  }
                }
                break;

            case 1:
                retrig_3 = (!lastGate_3 && gate_3) && !stillDelaying_3;
                if (retrig_3) LFOphase_3 = 0.0f;
                if (!stillDelaying_3) {
                    LFOphase_3 += phaseIncrement;
                    if (LFOphase_3 >= 1.0f) LFOphase_3 -= 1.0f;
                }
                lastGate_3 = gate_3;
                break;

            case 2: // RISE
                if (!stillDelaying_3 && !LFOstop_3) {
                    LFOonce_3 += LFOonceIncrement;
                    if (LFOonce_3 >= 1.0f) {
                        LFOonce_3 = 1.0f;
                        LFOstop_3 = true;
                        LFOactive_3 = false;
                    }
                }
                lfoOut_3 = LFOonce_3;
                break;

            case 3: // FALL
                if (!stillDelaying_3 && !LFOstop_3) {
                    LFOonce_3 -= LFOonceIncrement;
                    if (LFOonce_3 <= 0.0f) {
                        LFOonce_3 = 0.0f;
                        LFOstop_3 = true;
                        LFOactive_3 = false;
                    }
                }
                lfoOut_3 = LFOonce_3;
                break;

            default:
                retrig_3 = false;
                break;
        }
    }

    // === Voice 4 ===
    if (voiceActive_4 || LFOactive_4 || voiceReleasing_4) {
        bool stillDelaying_4 = false;
        if (LFOdelaying_4) {
            if ((unsigned long)lfoTimer >= LFOdelayEnd_4) {
                LFOdelaying_4 = false;
            } else {
                stillDelaying_4 = true;
            }
        }

        bool oneShotRetrig_4 = (LFOmodeSelect == 2 || LFOmodeSelect == 3) && gate_4 && !stillDelaying_4;
        if (oneShotRetrig_4) {
            LFOonce_4 = (LFOmodeSelect == 2) ? 0.0f : 1.0f;
            LFOstop_4 = false;
            LFOactive_4 = true;
        }

        bool retrig_4 = false;
        switch (LFOmodeSelect) {
            case 0:
                retrig_4 = false;    
                
                if (SynthMode == 0) {
                  if (!stillDelaying_4) {
                    if (unisonTriad == 1) LFOglobalPhase += phaseIncrement;
                    if (LFOglobalPhase >= 1.0f) LFOglobalPhase -= 1.0f;
                  }
                } else if (SynthMode == 1) {
                  if (!stillDelaying_4) {
                    if (LFOphase_1 >= 1.0f) LFOphase_1 -= 1.0f;
                  }
                }
                break;

            case 1:
                retrig_4 = (!lastGate_4 && gate_4) && !stillDelaying_4;
                if (retrig_4) LFOphase_4 = 0.0f;
                if (!stillDelaying_4) {
                    LFOphase_4 += phaseIncrement;
                    if (LFOphase_4 >= 1.0f) LFOphase_4 -= 1.0f;
                }
                lastGate_4 = gate_4;
                break;

            case 2: // RISE
                if (!stillDelaying_4 && !LFOstop_4) {
                    LFOonce_4 += LFOonceIncrement;
                    if (LFOonce_4 >= 1.0f) {
                        LFOonce_4 = 1.0f;
                        LFOstop_4 = true;
                        LFOactive_4 = false;
                    }
                }
                lfoOut_4 = LFOonce_4;
                break;

            case 3: // FALL
                if (!stillDelaying_4 && !LFOstop_4) {
                    LFOonce_4 -= LFOonceIncrement;
                    if (LFOonce_4 <= 0.0f) {
                        LFOonce_4 = 0.0f;
                        LFOstop_4 = true;
                        LFOactive_4 = false;
                    }
                }
                lfoOut_4 = LFOonce_4;
                break;

            default:
                retrig_4 = false;
                break;
        }
    }

    // === Voice 5 ===
    if (voiceActive_5 || LFOactive_5 || voiceReleasing_5) {
        bool stillDelaying_5 = false;
        if (LFOdelaying_5) {
            if ((unsigned long)lfoTimer >= LFOdelayEnd_5) {
                LFOdelaying_5 = false;
            } else {
                stillDelaying_5 = true;
            }
        }

        bool oneShotRetrig_5 = (LFOmodeSelect == 2 || LFOmodeSelect == 3) && gate_5 && !stillDelaying_5;
        if (oneShotRetrig_5) {
            LFOonce_5 = (LFOmodeSelect == 2) ? 0.0f : 1.0f;
            LFOstop_5 = false;
            LFOactive_5 = true;
        }

        bool retrig_5 = false;
        switch (LFOmodeSelect) {
            case 0:
                retrig_5 = false;
                if (SynthMode == 0) {
                  if (!stillDelaying_5) {
                    if (LFOglobalPhase >= 1.0f) LFOglobalPhase -= 1.0f;
                  }
                } else if (SynthMode == 1) {
                  if (!stillDelaying_5) {
                    if (LFOphase_1 >= 1.0f) LFOphase_1 -= 1.0f;
                  }
                }
                break;

            case 1:
                retrig_5 = (!lastGate_5 && gate_5) && !stillDelaying_5;
                if (retrig_5) LFOphase_5 = 0.0f;
                if (!stillDelaying_5) {
                    LFOphase_5 += phaseIncrement;
                    if (LFOphase_5 >= 1.0f) LFOphase_5 -= 1.0f;
                }
                lastGate_5 = gate_5;
                break;

            case 2: // RISE
                if (!stillDelaying_5 && !LFOstop_5) {
                    LFOonce_5 += LFOonceIncrement;
                    if (LFOonce_5 >= 1.0f) {
                        LFOonce_5 = 1.0f;
                        LFOstop_5 = true;
                        LFOactive_5 = false;
                    }
                }
                lfoOut_5 = LFOonce_5;
                break;

            case 3: // FALL
                if (!stillDelaying_5 && !LFOstop_5) {
                    LFOonce_5 -= LFOonceIncrement;
                    if (LFOonce_5 <= 0.0f) {
                        LFOonce_5 = 0.0f;
                        LFOstop_5 = true;
                        LFOactive_5 = false;
                    }
                }
                lfoOut_5 = LFOonce_5;
                break;

            default:
                retrig_5 = false;
                break;
        }
    }

    // === Voice 6 ===
    if (voiceActive_6 || LFOactive_6 || voiceReleasing_6) {
        bool stillDelaying_6 = false;
        if (LFOdelaying_6) {
            if ((unsigned long)lfoTimer >= LFOdelayEnd_6) {
                LFOdelaying_6 = false;
            } else {
                stillDelaying_6 = true;
            }
        }

        bool oneShotRetrig_6 = (LFOmodeSelect == 2 || LFOmodeSelect == 3) && gate_6 && !stillDelaying_6;
        if (oneShotRetrig_6) {
            LFOonce_6 = (LFOmodeSelect == 2) ? 0.0f : 1.0f;
            LFOstop_6 = false;
            LFOactive_6 = true;
        }

        bool retrig_6 = false;
        switch (LFOmodeSelect) {
            case 0:
                retrig_6 = false;
                if (SynthMode == 0) {
                  if (!stillDelaying_6) {
                    if (LFOglobalPhase >= 1.0f) LFOglobalPhase -= 1.0f;
                  }
                } else if (SynthMode == 1) {
                  if (!stillDelaying_6) {
                    if (LFOphase_1 >= 1.0f) LFOphase_1 -= 1.0f;
                  }
                }
                break;

            case 1:
                retrig_6 = (!lastGate_6 && gate_6) && !stillDelaying_6;
                if (retrig_6) LFOphase_6 = 0.0f;
                if (!stillDelaying_6) {
                    LFOphase_6 += phaseIncrement;
                    if (LFOphase_6 >= 1.0f) LFOphase_6 -= 1.0f;
                }
                lastGate_6 = gate_6;
                break;

            case 2: // RISE
                if (!stillDelaying_6 && !LFOstop_6) {
                    LFOonce_6 += LFOonceIncrement;
                    if (LFOonce_6 >= 1.0f) {
                        LFOonce_6 = 1.0f;
                        LFOstop_6 = true;
                        LFOactive_6 = false;
                    }
                }
                lfoOut_6 = LFOonce_6;
                break;

            case 3: // FALL
                if (!stillDelaying_6 && !LFOstop_6) {
                    LFOonce_6 -= LFOonceIncrement;
                    if (LFOonce_6 <= 0.0f) {
                        LFOonce_6 = 0.0f;
                        LFOstop_6 = true;
                        LFOactive_6 = false;
                    }
                }
                lfoOut_6 = LFOonce_6;
                break;

            default:
                retrig_6 = false;
                break;
        }
    }

  if (!LFOdelaying_1) gate_1 = false;
  if (!LFOdelaying_2) gate_2 = false;
  if (!LFOdelaying_3) gate_3 = false;
  if (!LFOdelaying_4) gate_4 = false;
  if (!LFOdelaying_5) gate_5 = false;
  if (!LFOdelaying_6) gate_6 = false;

  if (LFOmodeSelect == 0) {
    if (SynthMode == 0) {
      // Wave Lookup
      int index_1 = (int)(LFOglobalPhase * LFO_TABLE_SIZE);
      if (index_1 >= LFO_TABLE_SIZE) index_1 = LFO_TABLE_SIZE - 1;
      
      int index_2 = (int)(LFOglobalPhase * LFO_TABLE_SIZE);
      if (index_2 >= LFO_TABLE_SIZE) index_2 = LFO_TABLE_SIZE - 1;
      
      int index_3 = (int)(LFOglobalPhase * LFO_TABLE_SIZE);
      if (index_3 >= LFO_TABLE_SIZE) index_3 = LFO_TABLE_SIZE - 1;    

      int index_4 = (int)(LFOglobalPhase * LFO_TABLE_SIZE);
      if (index_4 >= LFO_TABLE_SIZE) index_4 = LFO_TABLE_SIZE - 1;

      int index_5 = (int)(LFOglobalPhase * LFO_TABLE_SIZE);
      if (index_5 >= LFO_TABLE_SIZE) index_5 = LFO_TABLE_SIZE - 1;
      
      int index_6 = (int)(LFOglobalPhase * LFO_TABLE_SIZE);
      if (index_6 >= LFO_TABLE_SIZE) index_6 = LFO_TABLE_SIZE - 1;

      const int16_t* selectedLFOTable;

      switch (LFOwaveSelect) {
          case 0: // sine
              selectedLFOTable = LFOTables[0];
              break;
          case 1: // sawRISE
              selectedLFOTable = LFOTables[1];
              break;
          case 2: // sawFALL
              selectedLFOTable = LFOTables[2];
              break;    
          case 3: // square
              selectedLFOTable = LFOTables[3];
              break;   
          case 4: // noise
              selectedLFOTable = LFOTables[4];
              break;     
          default: // fallback - sine
              selectedLFOTable = LFOTables[0];
              break;
      }

      int16_t raw_1 = selectedLFOTable[index_1];  // -32768 to 32767
      lfoOut_1 = (raw_1 + 32768) / 65535.0f;      // normalised to range 0.0 – 1.0
      
      int16_t raw_2 = selectedLFOTable[index_2];
      lfoOut_2 = (raw_2 + 32768) / 65535.0f;
      
      int16_t raw_3 = selectedLFOTable[index_3];
      lfoOut_3 = (raw_3 + 32768) / 65535.0f;
      
      int16_t raw_4 = selectedLFOTable[index_4];
      lfoOut_4 = (raw_4 + 32768) / 65535.0f;

      int16_t raw_5 = selectedLFOTable[index_5];
      lfoOut_5 = (raw_5 + 32768) / 65535.0f;
      
      int16_t raw_6 = selectedLFOTable[index_6];
      lfoOut_6 = (raw_6 + 32768) / 65535.0f;
    } 
    else {
      // Wave Lookup
      int index_1 = (int)(LFOphase_1 * LFO_TABLE_SIZE);
      if (index_1 >= LFO_TABLE_SIZE) index_1 = LFO_TABLE_SIZE - 1;
      
      int index_2 = (int)(LFOphase_1 * LFO_TABLE_SIZE);
      if (index_2 >= LFO_TABLE_SIZE) index_2 = LFO_TABLE_SIZE - 1;
      
      int index_3 = (int)(LFOphase_1 * LFO_TABLE_SIZE);
      if (index_3 >= LFO_TABLE_SIZE) index_3 = LFO_TABLE_SIZE - 1;
      
      int index_4 = (int)(LFOphase_1 * LFO_TABLE_SIZE);
      if (index_4 >= LFO_TABLE_SIZE) index_4 = LFO_TABLE_SIZE - 1;

      int index_5 = (int)(LFOphase_1 * LFO_TABLE_SIZE);
      if (index_5 >= LFO_TABLE_SIZE) index_5 = LFO_TABLE_SIZE - 1;
      
      int index_6 = (int)(LFOphase_1 * LFO_TABLE_SIZE);
      if (index_6 >= LFO_TABLE_SIZE) index_6 = LFO_TABLE_SIZE - 1;

      const int16_t* selectedLFOTable;

      switch (LFOwaveSelect) {
          case 0: // sine
              selectedLFOTable = LFOTables[0];
              break;
          case 1: // sawRISE
              selectedLFOTable = LFOTables[1];
              break;
          case 2: // sawFALL
              selectedLFOTable = LFOTables[2];
              break;    
          case 3: // square
              selectedLFOTable = LFOTables[3];
              break;   
          case 4: // noise
              selectedLFOTable = LFOTables[4];
              break;     
          default: // fallback - sine
              selectedLFOTable = LFOTables[0];
              break;
      }

      int16_t raw_1 = selectedLFOTable[index_1];  // -32768 to 32767
      lfoOut_1 = (raw_1 + 32768) / 65535.0f;      // normalised to range 0.0 – 1.0
      
      int16_t raw_2 = selectedLFOTable[index_2];
      lfoOut_2 = (raw_2 + 32768) / 65535.0f;
      
      int16_t raw_3 = selectedLFOTable[index_3];
      lfoOut_3 = (raw_3 + 32768) / 65535.0f;
      
      int16_t raw_4 = selectedLFOTable[index_4];
      lfoOut_4 = (raw_4 + 32768) / 65535.0f;

      int16_t raw_5 = selectedLFOTable[index_5];
      lfoOut_5 = (raw_5 + 32768) / 65535.0f;
      
      int16_t raw_6 = selectedLFOTable[index_6];
      lfoOut_6 = (raw_6 + 32768) / 65535.0f;
    }
  }

  if (LFOmodeSelect == 1) {
    int index_1 = (int)(LFOphase_1 * LFO_TABLE_SIZE);
    if (index_1 >= LFO_TABLE_SIZE) index_1 = LFO_TABLE_SIZE - 1;
    
    int index_2 = (int)(LFOphase_2 * LFO_TABLE_SIZE);
    if (index_2 >= LFO_TABLE_SIZE) index_2 = LFO_TABLE_SIZE - 1;
    
    int index_3 = (int)(LFOphase_3 * LFO_TABLE_SIZE);
    if (index_3 >= LFO_TABLE_SIZE) index_3 = LFO_TABLE_SIZE - 1;
    
    int index_4 = (int)(LFOphase_4 * LFO_TABLE_SIZE);
    if (index_4 >= LFO_TABLE_SIZE) index_4 = LFO_TABLE_SIZE - 1;

    int index_5 = (int)(LFOphase_5 * LFO_TABLE_SIZE);
    if (index_5 >= LFO_TABLE_SIZE) index_5 = LFO_TABLE_SIZE - 1;
    
    int index_6 = (int)(LFOphase_6 * LFO_TABLE_SIZE);
    if (index_6 >= LFO_TABLE_SIZE) index_6 = LFO_TABLE_SIZE - 1;

    const int16_t* selectedLFOTable;

    switch (LFOwaveSelect) {
          case 0: // sine
              selectedLFOTable = LFOTables[0];
              break;
          case 1: // sawRISE
              selectedLFOTable = LFOTables[1];
              break;
          case 2: // sawFALL
              selectedLFOTable = LFOTables[2];
              break;    
          case 3: // square
              selectedLFOTable = LFOTables[3];
              break;   
          case 4: // noise
              selectedLFOTable = LFOTables[4];
              break;     
          default: // fallback - sine
              selectedLFOTable = LFOTables[0];
              break;
    }

    int16_t raw_1 = selectedLFOTable[index_1];  // -32768 to 32767
    lfoOut_1 = (raw_1 + 32768) / 65535.0f;      // normalised to range 0.0 – 1.0
    
    int16_t raw_2 = selectedLFOTable[index_2];
    lfoOut_2 = (raw_2 + 32768) / 65535.0f;
    
    int16_t raw_3 = selectedLFOTable[index_3];
    lfoOut_3 = (raw_3 + 32768) / 65535.0f;
    
    int16_t raw_4 = selectedLFOTable[index_4];
    lfoOut_4 = (raw_4 + 32768) / 65535.0f;

    int16_t raw_5 = selectedLFOTable[index_5];
    lfoOut_5 = (raw_5 + 32768) / 65535.0f;
    
    int16_t raw_6 = selectedLFOTable[index_6];
    lfoOut_6 = (raw_6 + 32768) / 65535.0f;
  }

  // --- INHIBIT NON-ACTIVE VOICES (but allow ONESHOT/RELEASE LFO if still running) ---
  if (!voiceActive_1 && !LFOactive_1 && !voiceReleasing_1) lfoOut_1 = 0.0f;
  if (!voiceActive_2 && !LFOactive_2 && !voiceReleasing_2) lfoOut_2 = 0.0f;
  if (!voiceActive_3 && !LFOactive_3 && !voiceReleasing_3) lfoOut_3 = 0.0f;
  if (!voiceActive_4 && !LFOactive_4 && !voiceReleasing_4) lfoOut_4 = 0.0f;
  if (!voiceActive_5 && !LFOactive_5 && !voiceReleasing_5) lfoOut_5 = 0.0f;
  if (!voiceActive_6 && !LFOactive_6 && !voiceReleasing_6) lfoOut_6 = 0.0f;

    // Application of modulation (LFO output)
    switch (LFOtypeSelect) {
        case 0: { // freq
            // Modulates cutoff relative to the default value
            float modRange = 2.0f;  // ±100 % změna
            float mod_1 = 1.0f + modRange * LFOdepth * (lfoOut_1 - 0.5f);
            float mod_2 = 1.0f + modRange * LFOdepth * (lfoOut_2 - 0.5f);
            float mod_3 = 1.0f + modRange * LFOdepth * (lfoOut_3 - 0.5f);
            float mod_4 = 1.0f + modRange * LFOdepth * (lfoOut_4 - 0.5f);
            float mod_5 = 1.0f + modRange * LFOdepth * (lfoOut_5 - 0.5f);
            float mod_6 = 1.0f + modRange * LFOdepth * (lfoOut_6 - 0.5f);

            // NaN/Inf protection
            if (!isfinite(mod_1)) mod_1 = 1.0f;
            if (!isfinite(mod_2)) mod_2 = 1.0f;
            if (!isfinite(mod_3)) mod_3 = 1.0f;
            if (!isfinite(mod_4)) mod_4 = 1.0f;
            if (!isfinite(mod_5)) mod_5 = 1.0f;
            if (!isfinite(mod_6)) mod_6 = 1.0f;

            // Set proper safe range (avoid extremes)
            mod_1 = constrain(mod_1, 0.001f, 1.001f);
            mod_2 = constrain(mod_2, 0.001f, 1.001f);
            mod_3 = constrain(mod_3, 0.001f, 1.001f);
            mod_4 = constrain(mod_4, 0.001f, 1.001f);
            mod_5 = constrain(mod_5, 0.001f, 1.001f);
            mod_6 = constrain(mod_6, 0.001f, 1.001f);
            
            LFOfreqCutoffMod_1 = mod_1;
            LFOfreqCutoffMod_2 = mod_2;
            LFOfreqCutoffMod_3 = mod_3;
            LFOfreqCutoffMod_4 = mod_4;
            LFOfreqCutoffMod_5 = mod_5;
            LFOfreqCutoffMod_6 = mod_6;
            
            if ((SynthMode == 0) && (unisonTriad == 1)) {
              LFOfreqCutoffMod_7 = mod_4;
            } else {
              LFOfreqCutoffMod_7 = mod_1;
            }
            break;
        }

        case 1: {
            // Erase the Cutoff modulation caused by LFOtypeSelect == 0
            LFOfreqCutoffMod_1 = 1.0f;
            LFOfreqCutoffMod_2 = 1.0f;
            LFOfreqCutoffMod_3 = 1.0f;
            LFOfreqCutoffMod_4 = 1.0f;
            LFOfreqCutoffMod_5 = 1.0f;
            LFOfreqCutoffMod_6 = 1.0f;
            
            if ((SynthMode == 0) && (unisonTriad == 1)) {
              LFOfreqCutoffMod_7 = 1.0f;
            } else {
              LFOfreqCutoffMod_7 = 1.0f;
            }

            // ---------- WAVEFORM 1 ----------
            if (voiceActive_1 || LFOactive_1 || voiceReleasing_1) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_1];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                // invert rise/fall
                float offset = -LFOdepth * lfoOut_1 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;   // invert the value
                }

                // invert shape potentiometer direction
                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_1) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_1 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_1[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }
                
                drawOscBlock_fast(0);
                drawParams();

                waveform_1.arbitraryWaveform(myWaveform_1, TABLE_SIZE);
                waveformSub.arbitraryWaveform(myWaveform_1, TABLE_SIZE);
            }

            // ---------- WAVEFORM 2 ----------
            if (voiceActive_2 || LFOactive_2 || voiceReleasing_2) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_2];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_2 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_2) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_2 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_2[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }
                
                drawOscBlock_fast(1);
                drawParams();

                waveform_2.arbitraryWaveform(myWaveform_2, TABLE_SIZE);
            }

            // ---------- WAVEFORM 3 ----------
            if (voiceActive_3 || LFOactive_3 || voiceReleasing_3) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_3 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_3 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_3[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }

                drawOscBlock_fast(2);
                drawParams();

                waveform_3.arbitraryWaveform(myWaveform_3, TABLE_SIZE);
            }

            // ---------- WAVEFORM 1 - calculated separately for waveformClone_1 UNISON ----------
            if ((SynthMode == 0) && (voiceActive_4 || LFOactive_4 || voiceReleasing_4)) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_1];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_4 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_1) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_1 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_4[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }

                drawOscBlock_fast(0);
                drawParams();

                waveformClone_1.arbitraryWaveform(myWaveform_4, TABLE_SIZE);
                waveformSub.arbitraryWaveform(myWaveform_4, TABLE_SIZE);     
            }

            // ---------- WAVEFORM 2 - calculated separately for waveformClone_2 UNISON ----------
            if ((SynthMode == 0) && (voiceActive_5 || LFOactive_5 || voiceReleasing_5)) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_2];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_5 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_2) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_2 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_5[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }
                
                drawOscBlock_fast(1);
                drawParams();

                waveformClone_2.arbitraryWaveform(myWaveform_5, TABLE_SIZE);
            }

            // ---------- WAVEFORM 3 - calculated separately for waveformClone_3 UNISON ----------
            if ((SynthMode == 0) && (voiceActive_6 || LFOactive_6 || voiceReleasing_6)) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_6 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_3 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_6[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }

                drawOscBlock_fast(2);
                drawParams();

                waveformClone_3.arbitraryWaveform(myWaveform_6, TABLE_SIZE);
            }

            // ---------- WAVEFORM 3 - calculated separately for waveformClone_1 POLY ----------
            if ((SynthMode == 1) && (voiceActive_4 || LFOactive_4 || voiceReleasing_4)) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_4 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_3 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_4[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }
                waveformClone_1.arbitraryWaveform(myWaveform_4, TABLE_SIZE);
            }

            // ---------- WAVEFORM 3 - calculated separately for waveformClone_2 POLY ----------
            if ((SynthMode == 1) && (voiceActive_5 || LFOactive_5 || voiceReleasing_5)) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_5 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_3 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_5[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }
                waveformClone_2.arbitraryWaveform(myWaveform_5, TABLE_SIZE);
            }

            // ---------- WAVEFORM 3 - calculated separately for waveformClone_3 POLY ----------
            if ((SynthMode == 1) && (voiceActive_6 || LFOactive_6 || voiceReleasing_6)) {
                const WaveFamily& wf = waveFamilies[currentWaveFamily_3];
                int maxShapeIndex = wf.morphSteps * (wf.numWaves - 1);
                int morphSteps = wf.morphSteps;

                float offset = -LFOdepth * lfoOut_6 * maxShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    offset = -offset;
                }

                int modulatedShapeIndex;
                if ((LFOmodeSelect == 2 || LFOmodeSelect == 3) && LFOtypeSelect == 1) {
                    modulatedShapeIndex = constrain((int)((maxShapeIndex - currentShapeIndex_3) + offset), 0, maxShapeIndex);
                } else {
                    modulatedShapeIndex = constrain((int)(currentShapeIndex_3 + offset), 0, maxShapeIndex);
                }

                int baseIndex = modulatedShapeIndex / morphSteps;
                int step = modulatedShapeIndex % morphSteps;
                float t = step / (float)morphSteps;

                const int16_t* waveA = wf.waves[baseIndex];
                const int16_t* waveB = wf.waves[min(baseIndex + 1, wf.numWaves - 1)];

                for (int i = 0; i < TABLE_SIZE; i++) {
                    float fa = (float)waveA[i];
                    float fb = (float)waveB[i];
                    myWaveform_6[i] = (int16_t)((1.0f - t) * fa + t * fb);
                }
                waveformClone_3.arbitraryWaveform(myWaveform_6, TABLE_SIZE);
            }
            break;
        }

        case 2: {
            // Erase the Cutoff modulation caused by LFOtypeSelect == 0
            LFOfreqCutoffMod_1 = 1.0f;
            LFOfreqCutoffMod_2 = 1.0f;
            LFOfreqCutoffMod_3 = 1.0f;
            LFOfreqCutoffMod_4 = 1.0f;
            LFOfreqCutoffMod_5 = 1.0f;
            LFOfreqCutoffMod_6 = 1.0f;
            
            if ((SynthMode == 0) && (unisonTriad == 1)) {
              LFOfreqCutoffMod_7 = 1.0f;
            } else {
              LFOfreqCutoffMod_7 = 1.0f;
            }
            
            // defaults (if voice isnt active, set  Tune 0.5)
            float semitones_1 = 0.0f, semitones_2 = 0.0f, semitones_3 = 0.0f, semitones_4 = 0.0f, semitones_5 = 0.0f, semitones_6 = 0.0f;

            if (voiceActive_1 || LFOactive_1 || voiceReleasing_1) {
                semitones_1 = LFOTune_1 * LFOdepth * (lfoOut_1 * 2.0f - 1.0f);
                Tune_1 = powf(2.0f, semitones_1 / 12.0f);
            }

            if (voiceActive_2 || LFOactive_2 || voiceReleasing_2) {
                semitones_2 = LFOTune_2 * LFOdepth * (lfoOut_2 * 2.0f - 1.0f);
                Tune_2 = powf(2.0f, semitones_2 / 12.0f);
            }

            if (voiceActive_3 || LFOactive_3 || voiceReleasing_3) {
                semitones_3 = LFOTune_3 * LFOdepth * (lfoOut_3 * 2.0f - 1.0f);
                Tune_3 = powf(2.0f, semitones_3 / 12.0f);
            }

            if (voiceActive_4 || LFOactive_4 || voiceReleasing_4) {
                semitones_4 = LFOTune_4 * LFOdepth * (lfoOut_4 * 2.0f - 1.0f);
                Tune_4 = powf(2.0f, semitones_4 / 12.0f);
            }

            if (voiceActive_5 || LFOactive_5 || voiceReleasing_5) {
                semitones_5 = LFOTune_5 * LFOdepth * (lfoOut_5 * 2.0f - 1.0f);
                Tune_5 = powf(2.0f, semitones_5 / 12.0f);
            }

            if (voiceActive_6 || LFOactive_6 || voiceReleasing_6) {
                semitones_6 = LFOTune_6 * LFOdepth * (lfoOut_6 * 2.0f - 1.0f);
                Tune_6 = powf(2.0f, semitones_6 / 12.0f);
            }          
            break;
        }
        default:
            break;        
    }

    // --- cleanup releasing flags: if envelope finished, turn off Releasing, LFOactive and Active (+ mute the waveform) ---
    if (voiceReleasing_1 && !envelope_Amplitude_1.isActive()) {
        voiceReleasing_1 = false;
        LFOactive_1 = false;
        voiceActive_1 = false;
        waveform_1.amplitude(0);
    }
    if (voiceReleasing_2 && !envelope_Amplitude_2.isActive()) {
        voiceReleasing_2 = false;
        LFOactive_2 = false;
        voiceActive_2 = false;
        waveform_2.amplitude(0);
    }
    if (voiceReleasing_3 && !envelope_Amplitude_3.isActive()) {
        voiceReleasing_3 = false;
        LFOactive_3 = false;
        voiceActive_3 = false;
        waveform_3.amplitude(0);
    }
    if (voiceReleasing_4 && !envelopeClone_Amplitude_1.isActive()) {
        voiceReleasing_4 = false;
        LFOactive_4 = false;
        voiceActive_4 = false;
        waveformClone_1.amplitude(0);
    }
    if (voiceReleasing_5 && !envelopeClone_Amplitude_2.isActive()) {
        voiceReleasing_5 = false;
        LFOactive_5 = false;
        voiceActive_5 = false;
        waveformClone_2.amplitude(0);
    }
    if (voiceReleasing_6 && !envelopeClone_Amplitude_3.isActive()) {
        voiceReleasing_6 = false;
        LFOactive_6 = false;
        voiceActive_6 = false;
        waveformClone_3.amplitude(0);
    }
}