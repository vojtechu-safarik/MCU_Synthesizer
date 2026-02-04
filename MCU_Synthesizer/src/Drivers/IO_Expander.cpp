#include <Wire.h>
#include "PCF8574.h"   // Rob Tillaart library

// === IO Expander API ===
#include "Drivers/IO_Expander.h"
// ==================

// --- Configuration: expanders and mapping ---
// Addresses adjusted to the hardware. If an address is 0x00, that expander is disabled.
static constexpr uint8_t N_EXPANDERS = 2;
static constexpr uint8_t EXPANDER_ADDRS[N_EXPANDERS] = { 0x20, 0x22 };

// Logical LEDs are mapped across expanders: logical 0..7 => expander0 pins 0..7
// logical 8..15 => expander1 pins 0..7 (if present)
static constexpr uint8_t LEDS_PER_EXPANDER = 8;
static constexpr uint8_t MAX_LOGICAL_LEDS = N_EXPANDERS * LEDS_PER_EXPANDER;

// --- Runtime state ---
static PCF8574* pcfs[N_EXPANDERS] = { nullptr, nullptr };
static uint8_t stateCache[N_EXPANDERS] = { 0xFF, 0xFF };   // bit=1 => HIGH => LED OFF (sink wiring)
static uint8_t previousCache[N_EXPANDERS] = { 0xFF, 0xFF }; // last written value

// --- Internal helpers ---
static inline bool expanderAvailable(uint8_t e) {
  return (e < N_EXPANDERS) && (pcfs[e] != nullptr);
}

// Map logical index to expander and pin
static inline void mapLogicalToPhysical(uint8_t logical, uint8_t &expander, uint8_t &pin) {
  expander = logical / LEDS_PER_EXPANDER;
  pin = logical % LEDS_PER_EXPANDER;
}

// --- Public API implementation ---

void IOExpander_begin() {
  // Initialize appropriate I2C bus. 
  Wire.begin();

  // Create PCF objects for configured addresses
  for (uint8_t i = 0; i < N_EXPANDERS; ++i) {
    uint8_t addr = EXPANDER_ADDRS[i];
    if (addr != 0x00) {
      pcfs[i] = new PCF8574(addr);
      pcfs[i]->begin(0xFF); // all HIGH = LEDs OFF (sink)
      stateCache[i] = 0xFF;
      previousCache[i] = 0x00; // force first write
    } else {
      pcfs[i] = nullptr;
      stateCache[i] = 0xFF;
      previousCache[i] = 0xFF;
    }
  }
}

void IOExpander_setAllOff() {
  for (uint8_t i = 0; i < N_EXPANDERS; ++i) {
    stateCache[i] = 0xFF;
  }
}

void IOExpander_setLed(uint8_t idx, bool on) {
  if (idx >= MAX_LOGICAL_LEDS) return;
  uint8_t exp, pin;
  mapLogicalToPhysical(idx, exp, pin);
  if (!expanderAvailable(exp)) return;

  // PCF8574: bit = 0 => LOW => sink => LED ON
  if (on) {
    stateCache[exp] &= ~(1 << pin);  // set bit to 0 -> ON
  } else {
    stateCache[exp] |= (1 << pin);   // set bit to 1 -> OFF
  }
}

void IOExpander_update() {
  for (uint8_t i = 0; i < N_EXPANDERS; ++i) {
    if (!expanderAvailable(i)) continue;
    if (stateCache[i] != previousCache[i]) {
      pcfs[i]->write8(stateCache[i]);
      previousCache[i] = stateCache[i];
    }
  }
}
