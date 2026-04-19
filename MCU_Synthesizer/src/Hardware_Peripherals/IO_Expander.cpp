#include <Wire.h>

// === IO Expander API ===
#include "Hardware_Peripherals/IO_Expander.h"
// ==================

// --- Configuration ---
static constexpr uint8_t EXPANDER_ADDRS[2] = { 0x20, 0x22 };
static constexpr uint32_t I2C_SPEED = 100000;      // Standard speed for maximum reliability
static constexpr uint16_t UPDATE_INTERVAL_MS = 30; // 30ms refresh rate (~33Hz)

// --- State Management ---
// Initialize with 0xFF (All pins HIGH = LEDs OFF for sink wiring)
static uint8_t pcfCache[2] = { 0xFF, 0xFF };
static unsigned long lastRefreshTime = 0;

/**
 * @brief Initialize the IO Expanders and I2C bus
 */
void IOExpander_begin() {
    Wire.begin();
    Wire.setClock(I2C_SPEED);

    // Immediate hardware reset to OFF state
    for (uint8_t addr : EXPANDER_ADDRS) {
        Wire.beginTransmission(addr);
        Wire.write(0xFF);
        Wire.endTransmission();
    }
}

/**
 * @brief Set the logical state of an LED in the local cache
 * @param idx Logical index (0-7 for first expander, 8-15 for second)
 * @param on  True to turn LED ON (logic LOW), False to turn OFF (logic HIGH)
 */
void IOExpander_setLed(uint8_t idx, bool on) {
    if (idx > 15) return;

    uint8_t exp = (idx < 8) ? 0 : 1;
    uint8_t pin = (idx < 8) ? idx : (idx - 8);

    if (on) {
        pcfCache[exp] &= ~(1 << pin); // Clear bit (Sinks current -> LED ON)
    } else {
        pcfCache[exp] |= (1 << pin);  // Set bit (Logic HIGH -> LED OFF)
    }
}

/**
 * @brief Batch flush the cached state to the physical hardware
 * Uses a forced re-initialization of the Wire bus to prevent conflicts 
 * with other I2C peripherals.
 */
void IOExpander_update() {
    unsigned long currentTime = millis();

    // Limit update rate to prevent I2C bus congestion
    if (currentTime - lastRefreshTime >= UPDATE_INTERVAL_MS) {
        lastRefreshTime = currentTime;

        // CRITICAL: Re-initialize Wire bus before transmission.
        // This ensures the bus is in a known state even if other libraries
        // (like Display or Audio) have modified the I2C configuration.
        Wire.begin();
        Wire.setClock(I2C_SPEED);

        // Transmit cached states to both expanders
        for (uint8_t i = 0; i < 2; i++) {
            Wire.beginTransmission(EXPANDER_ADDRS[i]);
            Wire.write(pcfCache[i]);
            Wire.endTransmission();
        }
    }
}

/**
 * @brief Clear all LEDs in the cache
 */
void IOExpander_setAllOff() {
    pcfCache[0] = 0xFF;
    pcfCache[1] = 0xFF;
}