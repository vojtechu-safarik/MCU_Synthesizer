// === GLOBAL VARIABLES ===
#include "Global_Variables\Global_Variables.h"
// ==================

// === CONFIGURATION ===
#include "Configuration\Pins_Config.h"
// ==================

void checkSwitch() {
  // LFOtypeSelect
  /*
  if (digitalRead(SWLFOtype_1) && !digitalRead(SWLFOtype_3)) {
    LFOtypeSelect = 0;   // levá poloha
  }
  else if (!digitalRead(SWLFOtype_1) && digitalRead(SWLFOtype_3)) {
    LFOtypeSelect = 2;   // pravá poloha
  }
  else {
    LFOtypeSelect = 1;   // střed / žádná krajní (fallback)
  }
  */
  
// ================================
// ====== SEQUENCER Switches ======

  // SeqMode: 0 = Off, 1 = Arp, 2 = Latch
  if (digitalRead(SWSeqMode_1) && !digitalRead(SWSeqMode_3)) {
    CurrentSeqMode = 0;   // levá poloha // PAK MÁ BÝT NASTAVENO NA 0, SMAZAT, návrh teď jen
  }
  else if (!digitalRead(SWSeqMode_1) && digitalRead(SWSeqMode_3)) {
    CurrentSeqMode = 0;   // pravá poloha // PAK MÁ BÝT NASTAVENO NA 2, SMAZAT, návrh teď jen
  }
  else {
    CurrentSeqMode = 0;   // střed / žádná krajní (fallback) // PAK MÁ BÝT NASTAVENO NA 1, SMAZAT, návrh teď jen
  }

  // SeqOrder: 0 = Up, 1 = Down, 2 = Queue
  if (digitalRead(SWSeqOrder_1) && !digitalRead(SWSeqOrder_3)) {
    CurrentSeqOrder = 2;   // levá poloha // PAK MÁ BÝT NASTAVENO NA 0, SMAZAT, návrh teď jen
  }
  else if (!digitalRead(SWSeqOrder_1) && digitalRead(SWSeqOrder_3)) {
    CurrentSeqOrder = 2;   // pravá poloha // PAK MÁ BÝT NASTAVENO NA 2, SMAZAT, návrh teď jen
  }
  else {
    CurrentSeqOrder = 2;   // střed / žádná krajní (fallback) // PAK MÁ BÝT NASTAVENO NA 1, SMAZAT, návrh teď jen
  }

  // SeqOctaves: 0 = 1 oct, 1 = 2 oct, 2 = 3 oct
  if (digitalRead(SWSeqOctave_1) && !digitalRead(SWSeqOctave_3)) {
    CurrentSeqOctave = 0;   // levá poloha // PAK MÁ BÝT NASTAVENO NA 0, SMAZAT, návrh teď jen
  }
  else if (!digitalRead(SWSeqOctave_1) && digitalRead(SWSeqOctave_3)) {
    CurrentSeqOctave = 0;   // pravá poloha // PAK MÁ BÝT NASTAVENO NA 2, SMAZAT, návrh teď jen
  }
  else {
    CurrentSeqOctave = 0;   // střed / žádná krajní (fallback) // PAK MÁ BÝT NASTAVENO NA 1, SMAZAT, návrh teď jen
  }

}