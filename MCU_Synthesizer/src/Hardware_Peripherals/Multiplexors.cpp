// === GLOBAL VARIABLES ===
#include "Global_Variables/Global_Variables.h"
#include "Global_Variables/Wavetable_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/Pins_Config.h"
// ==================

// === CONFIGURATION ===
#include "Configuration/MIDI_Config.h"
// ==================

// === HARDWARE PERIPHERALS ===
#include "Hardware_Peripherals/Encoder_and_Multiplexors.h"
// ==================

// === SYNTHESIS FUNCTIONS ===
#include "Synthesis/Drone.h"
// ==================

void checkMux() {
  static byte muxInput = 0;
  
  // MIDI values arrays (0-127)
  static int MUX_1_values[MUX_1_control]; 
  static int MUX_2_values[MUX_2_control]; 
  static int MUX_3_values[MUX_3_control]; 

  // EMA filter state arrays
  static float MUX_1_ema[MUX_1_control] = {0};
  static float MUX_2_ema[MUX_2_control] = {0};
  static float MUX_3_ema[MUX_3_control] = {0};

  // Force initial state send on boot
  static bool isInitialized = false;
  if (!isInitialized) {
      for (int i = 0; i < MUX_1_control; i++) MUX_1_values[i] = -1;
      for (int i = 0; i < MUX_2_control; i++) MUX_2_values[i] = -1;
      for (int i = 0; i < MUX_3_control; i++) MUX_3_values[i] = -1;
      isInitialized = true;
  }

  unsigned long currentMicros = micros();
  static unsigned long lastTime = 0;

  // Three-way toggle switches (MUX only reads one channel at once)
  static bool SubOctave_Position_1 = false;
  static bool SubOctave_Position_3 = false;
  static bool LFOtype_1 = false;
  static bool LFOtype_3 = false;
  static bool SeqMode_1 = false;
  static bool SeqMode_3 = false;
  static bool SeqOrder_1 = false;
  static bool SeqOrder_3 = false;
  static bool SeqOctaves_1 = false;
  static bool SeqOctaves_3 = false;

  // run every 1000 µs (1 ms)
  if (currentMicros - lastTime >= 1000) {
    lastTime = currentMicros;

    // set MUX address pins
    digitalWrite(MUX_S0, muxInput & B0001);
    digitalWrite(MUX_S1, muxInput & B0010);
    digitalWrite(MUX_S2, muxInput & B0100);
    digitalWrite(MUX_S3, muxInput & B1000);

    // === MUX #1 ===
    if (muxInput < MUX_1_control) {
      int raw_read = analogRead(MUX_1_PIN);
      raw_read = 1023 - raw_read; // invert potentiometer values

      // EMA Filter
      MUX_1_ema[muxInput] = (raw_read * 0.2f) + (MUX_1_ema[muxInput] * 0.8f); // This ratio - the lower the coeff, the longer it stabilises but better it works
      int current_MIDI = ((int)MUX_1_ema[muxInput]) >> 3; // Change range to 0-127

      if (current_MIDI != MUX_1_values[muxInput]) {
        MUX_1_values[muxInput] = current_MIDI;
        int MUX_1_read = current_MIDI;
        
        switch (muxInput) {
          case MUXwave_1:
              VirtualControlChange(0, CCwave_1, (MUX_1_read * NUM_WAVEFAMILIES) / 128);
              break;  
          case MUXwave_2:
              VirtualControlChange(0, CCwave_2, (MUX_1_read * NUM_WAVEFAMILIES) / 128);
              break; 
          case MUXwave_3:
              VirtualControlChange(0, CCwave_3, (MUX_1_read * NUM_WAVEFAMILIES) / 128);
              break;         
          case MUXshape_1:
              VirtualControlChange(0, CCshape_1, MUX_1_read);
              break;
          case MUXshape_2:
              VirtualControlChange(0, CCshape_2, MUX_1_read);
              break;
          case MUXshape_3:
              VirtualControlChange(0, CCshape_3, MUX_1_read);
              break;
          case MUXvolume_1:
              VirtualControlChange(0, CCvolume_1, MUX_1_read);
              break;
          case MUXvolume_2:
              VirtualControlChange(0, CCvolume_2, MUX_1_read);
              break;
          case MUXvolume_3:
              VirtualControlChange(0, CCvolume_3, MUX_1_read);
              break;   
          case MUXtune_1:
              VirtualControlChange(0, CCtune_1, MUX_1_read);
              break;        
          case MUXtune_2:
              VirtualControlChange(0, CCtune_2, MUX_1_read);
              break;      
          case MUXtune_3:
              VirtualControlChange(0, CCtune_3, MUX_1_read);
              break;    
          case MUXnoise:
              VirtualControlChange(0, CCnoise, MUX_1_read);
              break;              
          case MUXsynthMode:
              SynthMode = (MUX_1_read < 64) ? 1 : 0;
              break; 
          case MUXsubOctave_1:
              SubOctave_Position_1 = (MUX_1_read < 64);
              break;
          case MUXsubOctave_3:
              SubOctave_Position_3 = (MUX_1_read < 64);
              break;      
        }

        if (SubOctave_Position_1 && !SubOctave_Position_3) {
            VirtualControlChange(0, CCsubOctave, 0);   // left position
        } else if (!SubOctave_Position_1 && SubOctave_Position_3) {
            VirtualControlChange(0, CCsubOctave, 2);   // right position
        } else {
            VirtualControlChange(0, CCsubOctave, 1);   // middle / no edge (fallback)
        }
      }  
    }

    // === MUX #2 ===
    if (muxInput < MUX_2_control) {
      int raw_read = analogRead(MUX_2_PIN);
      if (muxInput < 5) raw_read = 1023 - raw_read; // specific logic inversion

      // EMA Filter
      MUX_2_ema[muxInput] = (raw_read * 0.2f) + (MUX_2_ema[muxInput] * 0.8f);
      int current_MIDI = ((int)MUX_2_ema[muxInput]) >> 3; // Change range to 0-127

      if (current_MIDI != MUX_2_values[muxInput]) {
        MUX_2_values[muxInput] = current_MIDI;
        int MUX_2_read = current_MIDI;
        
        switch (muxInput) {
          case MUX_LPF_Cutoff:
              VirtualControlChange(0, CC_LPF_Cutoff, MUX_2_read);
              break;
          case MUX_LPF_Res:
              VirtualControlChange(0, CC_LPF_Res, MUX_2_read);
              break;
          case MUX_KBD_Track:
              VirtualControlChange(0, CC_KBD_Track, MUX_2_read);
              break;
          case MUX_EG_Int:
              VirtualControlChange(0, CC_EG_Int, MUX_2_read);
              break;    
          case MUX_HPF_Cutoff:
              VirtualControlChange(0, CC_HPF_Cutoff, MUX_2_read);
              break;  
          case MUX_Filter_Attack:
              VirtualControlChange(0, CC_Filter_Attack, MUX_2_read);
              break;  
          case MUX_Filter_Decay:
              VirtualControlChange(0, CC_Filter_Decay, MUX_2_read);
              break;  
          case MUX_Filter_Sustain:
              VirtualControlChange(0, CC_Filter_Sustain, MUX_2_read);
              break;  
          case MUX_Filter_Release:
              VirtualControlChange(0, CC_Filter_Release, MUX_2_read);
              break;  
          case MUX_LFO_Rate:
              VirtualControlChange(0, CC_LFO_Rate, MUX_2_read);
              break;  
          case MUX_LFO_Depth:
              VirtualControlChange(0, CC_LFO_Depth, MUX_2_read);
              break;  
          case MUX_LFO_Delay:
              VirtualControlChange(0, CC_LFO_Delay, MUX_2_read);
              break;  
          case MUX_Amplitude_Attack:
              VirtualControlChange(0, CC_Amplitude_Attack, MUX_2_read);
              break;  
          case MUX_Amplitude_Decay:
              VirtualControlChange(0, CC_Amplitude_Decay, MUX_2_read);
              break;  
          case MUX_Amplitude_Sustain:
              VirtualControlChange(0, CC_Amplitude_Sustain, MUX_2_read);
              break;  
          case MUX_Amplitude_Release:
              VirtualControlChange(0, CC_Amplitude_Release, MUX_2_read);
              break;  
        }
      }  
    }

    // === MUX #3 ===
    if (muxInput < MUX_3_control) {
      int raw_read = analogRead(MUX_3_PIN);
      raw_read = 1023 - raw_read; // invert potentiometer values

      // EMA Filter
      MUX_3_ema[muxInput] = (raw_read * 0.2f) + (MUX_3_ema[muxInput] * 0.8f);
      int current_MIDI = ((int)MUX_3_ema[muxInput]) >> 3; // Change range to 0-127

      if (current_MIDI != MUX_3_values[muxInput]) {
        MUX_3_values[muxInput] = current_MIDI;
        int MUX_3_read = current_MIDI;
        
        switch (muxInput) {
            case MUX_MasterVolume:
              VirtualControlChange(0, CC_MasterVolume, MUX_3_read);
              break; 
            case MUXSeqGatePot:
              VirtualControlChange(0, CCseqGatePot, MUX_3_read);
              break;   
          case MUX_LFOtype_1:
              LFOtype_1 = (MUX_3_read < 64);
              break;
          case MUX_LFOtype_3:
              LFOtype_3 = (MUX_3_read < 64);
              break;    
          case MUX_PORTswitch:
              if (SynthMode == 0) {
                PORTswitch = (MUX_3_read < 64) ? 1 : 0;
                unisonTriad = 1;                        
              } else {
                PORTswitch = 0;
                unisonTriad = 0;
              }
              break;            
          case MUX_Portamento_Mode:
                PORTmodeSelect = (MUX_3_read < 64) ? 1 : 0;
              break;
          case MUX_Drone_Switch:
                DRONEmodeSelect = (MUX_3_read < 64) ? 1 : 0;
              break;              
          case MUX_SeqMode_1:
              SeqMode_1 = (MUX_3_read < 64);
              break;
          case MUX_SeqMode_3:
              SeqMode_3 = (MUX_3_read < 64);
              break;    
          case MUX_SeqOrder_1:
              SeqOrder_1 = (MUX_3_read < 64);
              break;
          case MUX_SeqOrder_3:
              SeqOrder_3 = (MUX_3_read < 64);
              break;    
          case MUX_SeqOctaves_1:
              SeqOctaves_1 = (MUX_3_read < 64);
              break;
          case MUX_SeqOctaves_3:
              SeqOctaves_3 = (MUX_3_read < 64);
              break;    
        }  

        if (LFOtype_1 && !LFOtype_3) {
            LFOtypeSelect = 0;   // left position
        } else if (!LFOtype_1 && LFOtype_3) {
            LFOtypeSelect = 2;   // right position
        } else {
            LFOtypeSelect = 1;   // middle / no edge (fallback)
        }   
        
        if (SeqMode_1 && !SeqMode_3) {
            CurrentSeqMode = 0;   // left position
        } else if (!SeqMode_1 && SeqMode_3) {
            CurrentSeqMode = 2;   // right position
        } else {
            CurrentSeqMode = 1;   // middle / no edge (fallback)
        }

        if (SeqOrder_1 && !SeqOrder_3) {
            CurrentSeqOrder = 0;   // left position
        } else if (!SeqOrder_1 && SeqOrder_3) {
            CurrentSeqOrder = 2;   // right position
        } else {
            CurrentSeqOrder = 1;   // middle / no edge (fallback)
        }

        if (SeqOctaves_1 && !SeqOctaves_3) {
            CurrentSeqOctave = 0;   // left position
        } else if (!SeqOctaves_1 && SeqOctaves_3) {
            CurrentSeqOctave = 2;   // right position
        } else {
            CurrentSeqOctave = 1;   // middle / no edge (fallback)
        }
      }
    }    

    // switch to next MUX channel
    muxInput++;
    if (muxInput >= MUX_1_control) muxInput = 0;

    checkEncoder();  
  }
}