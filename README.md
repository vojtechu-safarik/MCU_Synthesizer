# Polyphonic Synthesizer for Teensy 4.1 

### Abstract
This project focuses on
[**Video demonstration of project**](add_here)

## List of hardware components

* Abc
  - what it does

## Circuit schematics
![Circuit schematics](images/add_here)

## Pinout Configuration

| Interface Group | Pin Name | Arduino Pin | AVR Port | Connected Components |
| :--- | :--- | :--- | :--- | :--- |
| **I2C Bus** | **SDA** | **A4** | PC4 | **LCD 1602** (Addr: `0x27`) <br> **BME280** (Addr: `0x76`) |
| | **SCL** | **A5** | PC5 | |
| **SPI Bus** | **SCK** | **D13** | PB5 | **SD Card Module** |
| | **MISO** | **D12** | PB4 | |
| | **MOSI** | **D11** | PB3 | |
| | **CS** | **D4** | PD4 | |
| **RTC (DS1302)** | CLK | **D8** | PB0 | **RTC** |
| | DAT | **D9** | PB1 | |
| | RST | **D10** | PB2 | |
| **Controls & UI** | CLK | **D5** | PD5 | **Rotary Encoder** |
| | DT | **D6** | PD6 | |
| | SW | **D7** | PD7 | |
| **Analog Sensors** | Analog | **A0** | PC0 | **Photoresistor** |

## Software design

## Project demonstration

### abc

abc

![Add_here](images/add_here)

## References

* Add here: link
