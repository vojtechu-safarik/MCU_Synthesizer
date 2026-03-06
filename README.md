# Polyphonic Synthesizer for Teensy 4.1 

### Abstract
This project is an advanced realization of bachelor's project at Brno Univesity of Technology. 
It creates a digital synthesizer using a microcontroller development board, encompassing code development and a prototype build (including PCBs).
The C++ code has been made in VS Code using PlatformIO and it utilises Teensy Audio Library. 
The scripts that generate wavetables (as a header file) have been made using Python.
Those header files have been further edited in order to lower the amount of anti-aliasing using a MATLAB script. 

[**Video demonstration of the Synthesizer**](add_here)

## Synthesis controls

![Synthesizer - front view](images/synthesizer_front_view)

sem jen obecně popsat bloky, lfo, drone a preset

## List of hardware components

* Abc
  - what it does

## Circuit & PCB schematics
![Main panel circuit schematics](images/main_panel_circuit_schematics)
![Keyboard panel circuit schematics](images/keyboard_panel_circuit_schematics)
![Main panel PCB schematics](images/main_panel_PCB_schematics)
![Keyboard panel PCB schematics](images/keyboard_panel_PCB_schematics)

## Pinout Configuration

| Teensy Pin | Pin name | Connected Components |
| :--- | :--- | :--- |
| 0 | **SW** | **Rotary Encoder** |
| 1 | **DT** | **Rotary Encoder** |
| 2 | **CLK** | **Rotary Encoder** |
| 3 | **S0** | **Mux #1, #2 and #3** |
| 4 | **S1** | **Mux #1, #2 and #3** |
| 5 | **S2** | **Mux #1, #2 and #3** |
| 6 | **S3** | **Mux #1, #2 and #3** |
| 7 | **DIN** | **Audioshield** |
| 8 | **DOUT** | **Audioshield** |
| 9 | **D/C** | **Display** |
| 10 | **CS** | **Display** |
| 11 | **SDI (MOSI)** | **Display** |
| 12 | **SDO (MISO)** | **Display** |
| 13 | **SCK** | **Display** |
| 14 | **Signal #1** | **LED #1** |
| 15 | **Signal #2** | **LED #2** |
| 16 | **Signal #3** | **LED #3** |
| 17 | **Signal #4** | **LED #4** |
| 18 | **SDA** | **Audioshield** |
| 19 | **SCL** | **Audioshield** |
| 20 | **LRCLK** | **Audioshield** |
| 21 | **BCLK** | **Audioshield** |
| 22 | **Signal #5** | **LED #5** |
| 23 | **MCLK** | **Audioshield** |
| 24 | **Column #1** | **Keyboard** |
| 25 | **Column #2** | **Keyboard** |
| 26 | **Column #3** | **Keyboard** |
| 27 | **Column #4** | **Keyboard** |
| 28 | **Column #5** | **Keyboard** |
| 29 | **Column #6** | **Keyboard** |
| 30 | **Column #7** | **Keyboard** |
| 31 | **Column #8** | **Keyboard** |
| 32 | **Column #9** | **Keyboard** |
| 33 | **Row #1** | **Keyboard** |
| 34 | **Row #2** | **Keyboard** |
| 35 | **Row #3** | **Keyboard** |
| 36 | **Row #4** | **Keyboard** |
| 37 | **Row #5** | **Keyboard** |
| 38 | **Signal #6** | **LED #6** |
| 39 | **Sig (A15)** | **Mux #1** |
| 40 | **Sig (A16)** | **Mux #2** |
| 41 | **Sig (A17)** | **Mux #3** |

### 3D Model

![3D Model - front view](images/3d_model_front_view)
![3D Model - side view](images/3d_model_side_view)
![3D Model - back view](images/3d_model_back_view)

byl vyrobenej pro návrh ovládacích panelů ke gravírování a dodržení správných rozměrů

## References

* Add here: link
