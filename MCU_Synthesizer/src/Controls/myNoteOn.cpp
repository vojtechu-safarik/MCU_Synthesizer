// ==========================================
// ====== Headers required by function ======

// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis\Teensy_Audio.h"
#include "Synthesis\LFO.h"
#include "Synthesis\KBD_Tracking.h"
// ==================

// === VOICES FUNCTIONS ===
#include "Voices\oscPlayVoice.h"
#include "Voices\oscStopVoice.h"
// ==================

// === CONTROLS ===
#include "Controls\MIDI_Control.h"
// ==================

// ----------- Keyboard -----------
void myNoteOn(byte channel, byte note, byte velocity) {
  // UNISON
  if (SynthMode == 0) {
    if (unisonTriadToggle == true) {
      unisonTriad ^= 1;     // toggle 0 <-> 1
    } else if (unisonTriadToggle == false) {
      unisonTriad = 1;      // only use the second triad of oscillators - first suffers more aliasing, i guess? - bug, didnt figure out yet
    }  
  // Serial.print("Current unisonTriad = "); Serial.println(unisonTriad);
    if (unisonTriad == 0) {
      if (PORTswitch == 0) {
        oscStopVoice_4();
        oscStopVoice_5();
        oscStopVoice_6();
        envelopeNoise_1.noteOff();
      }
      oscPlayVoice_1(note);
      oscPlayVoice_2(note);
      oscPlayVoice_3(note);
      voiceActive_1 = true;
      voiceNote_1 = note;
      voiceActive_2 = true;
      voiceNote_2 = note;
      voiceActive_3 = true;
      voiceNote_3 = note;
      updateKBDtrackingMods();

      // initialization PORTAMENTO - voices 1 .. 3
      if (PORTswitch == 1) {
        mixer_1.gain(0, 0.5);
        mixer_1.gain(1, 0.5);
        mixer_1.gain(2, 0.5);
        bool sourceHolding = ( (voiceActive_4 && !voiceReleasing_4)
                            || (voiceActive_5 && !voiceReleasing_5)
                            || (voiceActive_6 && !voiceReleasing_6) );

      { // VOICE 1
        float start1 = 0.0f;
        if (sourceHolding) {
          start1 = ((float)voiceNote_4 - (float)note) + PORTcurrentSemitone_4; // relative start towards a new note
        } else {
          start1 = 0.0f; // no source -> no glide (target = 0)
        }
        PORTstartSemitone_1 = start1;
        PORTtargetSemitone_1 = 0.0f;
        PORTcurrentSemitone_1 = PORTstartSemitone_1; // init for Fixed Rate and Fixed Time

        float delta1 = fabsf(PORTtargetSemitone_1 - PORTstartSemitone_1);
        if (delta1 < 0.0001f || !sourceHolding) {
          PORTactive_1 = false;
          PORTonce_1 = 1.0f;
          PORTcurrentSemitone_1 = PORTtargetSemitone_1;
          PORTstop_1 = true;
        } else {
          // Fixed Time: enable glide only if Time mode is set on and the source is being held
          if (PORTmodeSelect == 0) {
            PORTonce_1 = 0.0f;
            PORTstop_1 = false;
            PORTactive_1 = true;
          } else {
            if (sourceHolding) {
              PORTactive_1 = true;
              PORTstop_1 = false;
            } else {
              PORTactive_1 = false;
              PORTstop_1 = true;
              PORTcurrentSemitone_1 = PORTtargetSemitone_1;
              PORTonce_1 = 1.0f;
            }
          }
        }
      }
      
      { // VOICE 2
        float start2 = 0.0f;
        if (sourceHolding) {
          start2 = ((float)voiceNote_4 - (float)note) + PORTcurrentSemitone_4;
        } else {
          start2 = 0.0f;
        }  
        PORTstartSemitone_2 = start2;
        PORTtargetSemitone_2 = 0.0f;
        PORTcurrentSemitone_2 = PORTstartSemitone_2;
        
        float delta2 = fabsf(PORTtargetSemitone_2 - PORTstartSemitone_2);
        if (delta2 < 0.0001f || !sourceHolding) {
          PORTactive_2 = false; 
          PORTonce_2 = 1.0f; 
          PORTcurrentSemitone_2 = PORTtargetSemitone_2; 
          PORTstop_2 = true;
        } else {
          if (PORTmodeSelect == 0) { 
            PORTonce_2 = 0.0f; 
            PORTstop_2 = false; 
            PORTactive_2 = true; 
          } else { 
            if (sourceHolding) { 
              PORTactive_2 = true; 
              PORTstop_2 = false; 
            } else { 
              PORTactive_2 = false; 
              PORTstop_2 = true; 
              PORTcurrentSemitone_2 = PORTtargetSemitone_2; 
              PORTonce_2 = 1.0f; 
            } 
          }
        }
      }

      { // VOICE 3
        float start3 = 0.0f;
        if (sourceHolding) {
          start3 = ((float)voiceNote_4 - (float)note) + PORTcurrentSemitone_4;
        } else {
          start3 = 0.0f;
        }  
        PORTstartSemitone_3 = start3;
        PORTtargetSemitone_3 = 0.0f;
        PORTcurrentSemitone_3 = PORTstartSemitone_3;

        float delta3 = fabsf(PORTtargetSemitone_3 - PORTstartSemitone_3);
        if (delta3 < 0.0001f || !sourceHolding) {
          PORTactive_3 = false; 
          PORTonce_3 = 1.0f; 
          PORTcurrentSemitone_3 = PORTtargetSemitone_3; 
          PORTstop_3 = true;
        } else {
          if (PORTmodeSelect == 0) { 
            PORTonce_3 = 0.0f; 
            PORTstop_3 = false; 
            PORTactive_3 = true; 
          } else { 
            if (sourceHolding) { 
              PORTactive_3 = true; 
              PORTstop_3 = false; 
            } else { 
              PORTactive_3 = false; 
              PORTstop_3 = true; 
              PORTcurrentSemitone_3 = PORTtargetSemitone_3; 
              PORTonce_3 = 1.0f; 
            } 
          }
        }
      }

      mixer_2.gain(0, 0);
      mixer_2.gain(1, 0);
      mixer_2.gain(2, 0);
      }
      
      if (PORTswitch == 0) {
        mixer_1.gain(0, 0.5);
        mixer_1.gain(1, 0.5);
        mixer_1.gain(2, 0.5);
        mixer_2.gain(0, 0.5);
        mixer_2.gain(1, 0.5);
        mixer_2.gain(2, 0.5);
      }

      oscStopVoice_4();
      oscStopVoice_5();
      oscStopVoice_6();
      envelopeNoise_1.noteOff();

      // initialization LFO - voices 4 .. 6
      if (LFOmodeSelect == 1) { gate_1 = true; gate_2 = true; gate_3 = true; }
      if (LFOmodeSelect == 2) { LFOonce_1 = 0.0f; LFOstop_1 = false; LFOactive_1 = true; 
                                LFOonce_2 = 0.0f; LFOstop_2 = false; LFOactive_2 = true;
                                LFOonce_3 = 0.0f; LFOstop_3 = false; LFOactive_3 = true; }
      if (LFOmodeSelect == 3) { LFOonce_1 = 1.0f; LFOstop_1 = false; LFOactive_1 = true; 
                                LFOonce_2 = 1.0f; LFOstop_2 = false; LFOactive_2 = true;
                                LFOonce_3 = 1.0f; LFOstop_3 = false; LFOactive_3 = true; }

      if (LFOdelay > 0.0f) {
          LFOdelaying_1 = true; LFOdelaying_2 = true; LFOdelaying_3 = true;
          LFOdelayEnd_1 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
          LFOdelayEnd_2 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
          LFOdelayEnd_3 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_1 = false;
          LFOdelaying_2 = false;
          LFOdelaying_3 = false;
      }
      return;
    }

    if (unisonTriad == 1) {
      if (PORTswitch == 0) {
        oscStopVoice_1();
        oscStopVoice_2();
        oscStopVoice_3();
        envelopeNoise_1.noteOff();
      }
      oscPlayVoice_4(note);
      oscPlayVoice_5(note);
      oscPlayVoice_6(note);
      voiceActive_4 = true;
      voiceNote_4 = note;
      voiceActive_5 = true;
      voiceNote_5 = note;
      voiceActive_6 = true;
      voiceNote_6 = note;
      updateKBDtrackingMods();
      
      // initialization PORTAMENTO - voices 4 .. 6
      if (PORTswitch == 1) {
        mixer_2.gain(0, 0.5);
        mixer_2.gain(1, 0.5);
        mixer_2.gain(2, 0.5);
        bool sourceHolding = ( (voiceActive_1 && !voiceReleasing_1)
                            || (voiceActive_2 && !voiceReleasing_2)
                            || (voiceActive_3 && !voiceReleasing_3) );

        {
          // VOICE 4
          float start4 = 0.0f;
          if (sourceHolding) {
            start4 = ((float)voiceNote_1 - (float)note) + PORTcurrentSemitone_1;
          } else {
            start4 = 0.0f;
          }
          PORTstartSemitone_4 = start4;
          PORTtargetSemitone_4 = 0.0f;
          PORTcurrentSemitone_4 = PORTstartSemitone_4;

          float delta4 = fabsf(PORTtargetSemitone_4 - PORTstartSemitone_4);
          if (delta4 < 0.0001f || !sourceHolding) {
            PORTactive_4 = false;
            PORTonce_4 = 1.0f;
            PORTcurrentSemitone_4 = PORTtargetSemitone_4;
            PORTstop_4 = true;
          } else {
            if (PORTmodeSelect == 0) {
              PORTonce_4 = 0.0f;
              PORTstop_4 = false;
              PORTactive_4 = true;
            } else {
              if (sourceHolding) {
                PORTactive_4 = true;
                PORTstop_4 = false;
              } else {
                PORTactive_4 = false;
                PORTstop_4 = true;
                PORTcurrentSemitone_4 = PORTtargetSemitone_4;
                PORTonce_4 = 1.0f;
              }
            }
          }
        }

        { // VOICE 5
          float start5 = 0.0f;
          if (sourceHolding) {
            start5 = ((float)voiceNote_1 - (float)note) + PORTcurrentSemitone_1;
          }  
          else { 
            start5 = 0.0f;
          }
          PORTstartSemitone_5 = start5;
          PORTtargetSemitone_5 = 0.0f;
          PORTcurrentSemitone_5 = PORTstartSemitone_5;

          float delta5 = fabsf(PORTtargetSemitone_5 - PORTstartSemitone_5);
          if (delta5 < 0.0001f || !sourceHolding) {
            PORTactive_5 = false; 
            PORTonce_5 = 1.0f; 
            PORTcurrentSemitone_5 = PORTtargetSemitone_5; 
            PORTstop_5 = true;
          } else {
            if (PORTmodeSelect == 0) { 
              PORTonce_5 = 0.0f; 
              PORTstop_5 = false; 
              PORTactive_5 = true; 
            } else { 
              if (sourceHolding) { 
                PORTactive_5 = true; 
                PORTstop_5 = false; 
              } else { 
                PORTactive_5 = false; 
                PORTstop_5 = true; 
                PORTcurrentSemitone_5 = PORTtargetSemitone_5; 
                PORTonce_5 = 1.0f; 
              } 
            }
          }
        }

        { // VOICE 6
          float start6 = 0.0f;
          if (sourceHolding) {
            start6 = ((float)voiceNote_1 - (float)note) + PORTcurrentSemitone_1;
          } else {
            start6 = 0.0f;
          }  
          PORTstartSemitone_6 = start6;
          PORTtargetSemitone_6 = 0.0f;
          PORTcurrentSemitone_6 = PORTstartSemitone_6;

          float delta6 = fabsf(PORTtargetSemitone_6 - PORTstartSemitone_6);
          if (delta6 < 0.0001f || !sourceHolding) {
            PORTactive_6 = false; 
            PORTonce_6 = 1.0f; 
            PORTcurrentSemitone_6 = PORTtargetSemitone_6; 
            PORTstop_6 = true;
          } else {
            if (PORTmodeSelect == 0) { 
              PORTonce_6 = 0.0f; 
              PORTstop_6 = false; 
              PORTactive_6 = true; 
            } else { 
              if (sourceHolding) { 
                PORTactive_6 = true; 
                PORTstop_6 = false; 
              } else { 
                PORTactive_6 = false; 
                PORTstop_6 = true; 
                PORTcurrentSemitone_6 = PORTtargetSemitone_6; 
                PORTonce_6 = 1.0f; 
              } 
            }
          }
        } 

      mixer_1.gain(0, 0);
      mixer_1.gain(1, 0);
      mixer_1.gain(2, 0);
      }  

      if (PORTswitch == 0) {
        mixer_1.gain(0, 0.5);
        mixer_1.gain(1, 0.5);
        mixer_1.gain(2, 0.5);
        mixer_2.gain(0, 0.5);
        mixer_2.gain(1, 0.5);
        mixer_2.gain(2, 0.5);
      }

      oscStopVoice_1();
      oscStopVoice_2();
      oscStopVoice_3();
      envelopeNoise_1.noteOff();

      // initialization LFO - voices 4 .. 6
      if (LFOmodeSelect == 1) { gate_4 = true; gate_5 = true; gate_6 = true; }
      if (LFOmodeSelect == 2) { LFOonce_4 = 0.0f; LFOstop_4 = false; LFOactive_4 = true; 
                                LFOonce_5 = 0.0f; LFOstop_5 = false; LFOactive_5 = true;
                                LFOonce_6 = 0.0f; LFOstop_6 = false; LFOactive_6 = true; }
      if (LFOmodeSelect == 3) { LFOonce_4 = 1.0f; LFOstop_4 = false; LFOactive_4 = true; 
                                LFOonce_5 = 1.0f; LFOstop_5 = false; LFOactive_5 = true;
                                LFOonce_6 = 1.0f; LFOstop_6 = false; LFOactive_6 = true; }

      if (LFOdelay > 0.0f) {
          LFOdelaying_4 = true; LFOdelaying_5 = true; LFOdelaying_6 = true;
          LFOdelayEnd_4 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
          LFOdelayEnd_5 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
          LFOdelayEnd_6 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_4 = false;
          LFOdelaying_5 = false;
          LFOdelaying_6 = false;
      }
      return;
    }
  }
  
  if (SynthMode == 1) {
    // POLY: find an empty voice, set, initialize LFO only for the specific voice
    if (!voiceActive_1) {
      oscPlayVoice_1(note);
      // if oscPlayVoice_1 for some reason doesnt set these variables, they are repeated here
      voiceActive_1 = true;
      voiceNote_1 = note;
      updateKBDtrackingMods();

      // initialize LFO for voice 1
      if (LFOmodeSelect == 1) gate_1 = true;
      if (LFOmodeSelect == 2) { LFOonce_1 = 0.0f; LFOstop_1 = false; LFOactive_1 = true; }
      if (LFOmodeSelect == 3) { LFOonce_1 = 1.0f; LFOstop_1 = false; LFOactive_1 = true; }

      if (LFOdelay > 0.0f) {
          LFOdelaying_1 = true;
          LFOdelayEnd_1 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_1 = false;
      }
      return;
    }

    if (!voiceActive_2) {
      oscPlayVoice_2(note);
      voiceActive_2 = true;
      voiceNote_2 = note;
      updateKBDtrackingMods();

      // initialize LFO for voice 2
      if (LFOmodeSelect == 1) gate_2 = true;
      if (LFOmodeSelect == 2) { LFOonce_2 = 0.0f; LFOstop_2 = false; LFOactive_2 = true; }
      if (LFOmodeSelect == 3) { LFOonce_2 = 1.0f; LFOstop_2 = false; LFOactive_2 = true; }
      
      if (LFOdelay > 0.0f) {
          LFOdelaying_2 = true;
          LFOdelayEnd_2 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_2 = false;
      }
      return;
    }

    if (!voiceActive_3) {
      oscPlayVoice_3(note);
      voiceActive_3 = true;
      voiceNote_3 = note;
      updateKBDtrackingMods();

      // initialize LFO for voice 3
      if (LFOmodeSelect == 1) gate_3 = true;
      if (LFOmodeSelect == 2) { LFOonce_3 = 0.0f; LFOstop_3 = false; LFOactive_3 = true; }
      if (LFOmodeSelect == 3) { LFOonce_3 = 1.0f; LFOstop_3 = false; LFOactive_3 = true; }
      
      if (LFOdelay > 0.0f) {
          LFOdelaying_3 = true;
          LFOdelayEnd_3 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_3 = false;
      }
      return;
    }

    if (!voiceActive_4) {
      oscPlayVoice_4(note);
      voiceActive_4 = true;
      voiceNote_4 = note;
      updateKBDtrackingMods();

      // initialize LFO for voice 4
      if (LFOmodeSelect == 1) gate_4 = true;
      if (LFOmodeSelect == 2) { LFOonce_4 = 0.0f; LFOstop_4 = false; LFOactive_4 = true; }
      if (LFOmodeSelect == 3) { LFOonce_4 = 1.0f; LFOstop_4 = false; LFOactive_4 = true; }
      
      if (LFOdelay > 0.0f) {
          LFOdelaying_4 = true;
          LFOdelayEnd_4 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_4 = false;
      }
      return;
    }

    if (!voiceActive_5) {
      oscPlayVoice_5(note);
      voiceActive_5 = true;
      voiceNote_5 = note;
      updateKBDtrackingMods();

      // initialize LFO for voice 5
      if (LFOmodeSelect == 1) gate_5 = true;
      if (LFOmodeSelect == 2) { LFOonce_5 = 0.0f; LFOstop_5 = false; LFOactive_5 = true; }
      if (LFOmodeSelect == 3) { LFOonce_5 = 1.0f; LFOstop_5 = false; LFOactive_5 = true; }
      
      if (LFOdelay > 0.0f) {
          LFOdelaying_5 = true;
          LFOdelayEnd_5 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_5 = false;
      }
      return;
    }

      if (!voiceActive_6) {
      oscPlayVoice_6(note);
      voiceActive_6 = true;
      voiceNote_6 = note;
      updateKBDtrackingMods();

      // initialize LFO for voice 6
      if (LFOmodeSelect == 1) gate_6 = true;
      if (LFOmodeSelect == 2) { LFOonce_6 = 0.0f; LFOstop_6 = false; LFOactive_6 = true; }
      if (LFOmodeSelect == 3) { LFOonce_6 = 1.0f; LFOstop_6 = false; LFOactive_6 = true; }
      
      if (LFOdelay > 0.0f) {
          LFOdelaying_6 = true;
          LFOdelayEnd_6 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_6 = false;
      }
      return;
    }

    // fallback – use voice 6 (rewrite)
    oscPlayVoice_6(note);
    voiceActive_6 = true;
    voiceNote_6 = note;
    updateKBDtrackingMods();
    
    if (LFOmodeSelect == 1) gate_6 = true;
    if (LFOmodeSelect == 2) { LFOonce_6 = 0.0f; LFOstop_6 = false; LFOactive_6 = true; }
    if (LFOmodeSelect == 3) { LFOonce_6 = 1.0f; LFOstop_6 = false; LFOactive_6 = true; }
    
    if (LFOdelay > 0.0f) {
          LFOdelaying_6 = true;
          LFOdelayEnd_6 = (unsigned long)lfoTimer + (unsigned long)(LFOdelay * 1000.0f);
      } else {
          LFOdelaying_6 = false;
      }
    return;
  }
}  