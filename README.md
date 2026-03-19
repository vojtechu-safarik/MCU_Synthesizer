# Polyphonic Synthesizer for Teensy 4.1 

### Abstract
This project is an advanced realization of bachelor's project at Brno Univesity of Technology. 
It creates a digital synthesizer using a microcontroller development board, encompassing code development and a prototype build (including PCBs).
The C++ code has been made in VS Code using PlatformIO and it utilises Teensy Audio Library. 
The scripts that generate wavetables (as a header file) have been made using Python.
Those header files have been further edited in order to lower the amount of anti-aliasing using a MATLAB script. 

[**Video demonstration of the Synthesizer**](add_here)

## Synthesis controls

![Synthesizer - front view](Images/Synthesizer-front_view.png "Synthesizer - front view")

### Oscillators
abc

### Filters
abc

### Envelopes
abc

### LFO
abc

### Portamento
abc

### Sequencer
abc

### Drone
abc

### Preset
abc

## List of hardware components

* Teensy 4.1 Development board

* Audio Adaptor Board for Teensy (Rev D)

* 16-Channel Analog/Digital Multiplexer/Demultiplexer, CD74HC4067
  - used for various parameter configuration via potentiometers and toggle switches
  - 3 modules used in total 

  ### Mux 1 Channels:
  | Mux Channel | Parameter name | Connected Component |
  | :--- | :--- | :--- |
  | C0 | **Wave 1** | **Potentiometer** |
  | C1 | **Wave 2** | **Potentiometer** |
  | C2 | **Wave 3** | **Potentiometer** |
  | C3 | **Shape 1** | **Potentiometer** |
  | C4 | **Shape 2** | **Potentiometer** |
  | C5 | **Shape 3** | **Potentiometer** |
  | C6 | **Volume 1** | **Potentiometer** |
  | C7 | **Volume 2** | **Potentiometer** |
  | C8 | **Volume 3** | **Potentiometer** |
  | C9 | **Tune 1** | **Potentiometer** |
  | C10 | **Tune 2** | **Potentiometer** |
  | C11 | **Tune 3** | **Potentiometer** |
  | C12 | **Pink Noise** | **Potentiometer** |
  | C13 | **SynthMode - Poly** | **2-way Toggle Switch** |
  | C14 | **Sub-OSC - OFF** | **3-way Toggle Switch** |
  | C15 | **Sub-OSC - +2 octaves** | **3-way Toggle Switch** |

  ### Mux 2 Channels:
  | Mux Channel | Parameter name | Connected Component |
  | :--- | :--- | :--- |
  | C0 | **LPF Cutoff** | **Potentiometer** |
  | C1 | **LPF Resonance** | **Potentiometer** |
  | C2 | **Keyboard Tracking** | **Potentiometer** |
  | C3 | **Filter Envelope Intensity** | **Potentiometer** |
  | C4 | **HPF Cutoff** | **Potentiometer** |
  | C5 | **Filter Attack** | **Potentiometer** |
  | C6 | **Filter Decay** | **Potentiometer** |
  | C7 | **Filter Sustain** | **Potentiometer** |
  | C8 | **Filter Release** | **Potentiometer** |
  | C9 | **LFO Rate** | **Potentiometer** |
  | C10 | **LFO Depth** | **Potentiometer** |
  | C11 | **LFO Delay** | **Potentiometer** |
  | C12 | **Amplitude Attack** | **Potentiometer** |
  | C13 | **Amplitude Decay** | **Potentiometer** |
  | C14 | **Amplitude Sustain** | **Potentiometer** |
  | C15 | **Amplitude Release** | **Potentiometer** |

  ### Mux 3 Channels:
  | Mux Channel | Parameter name | Connected Component |
  | :--- | :--- | :--- |
  | C0 | **Master Volume** | **Potentiometer** |
  | C1 | **Sequencer Gate** | **Potentiometer** |
  | C2 | **LFO Type - Frequency** | **3-way Toggle Switch** |
  | C3 | **LFO Type - Tune** | **3-way Toggle Switch** |
  | C4 | **Portamento - ON** | **2-way Toggle Switch** |
  | C5 | **Portamento - Mode** | **2-way Toggle Switch** |
  | C6 | **Drone - mode** | **2-way Toggle Switch** |
  | C7 | **Sequencer Mode - OFF** | **3-way Toggle Switch** |
  | C8 | **Sequencer Mode - Latch** | **3-way Toggle Switch** |
  | C9 | **Sequencer Order - Up** | **3-way Toggle Switch** |
  | C10 | **Sequencer Order - Queue** | **3-way Toggle Switch** |
  | C11 | **Sequencer Octaves - 0** | **3-way Toggle Switch** |
  | C12 | **Sequencer Octaves - 2** | **3-way Toggle Switch** |

* 8-bit I/O Expander I2C-Bus, PCF8574T
  - used for LED signals for parameter state identification 
  - 2 modules used in total 

  ### I/O Expander 1 Pinout:
  | I/O Expander Pin | Parameter name |
  | :--- | :--- |
  | P0 | **LFO Mode #1** |
  | P1 | **LFO Mode #2** |
  | P2 | **LFO Mode #3** |
  | P3 | **LFO Mode #4** |
  | P4 | **LFO Wave #1** |
  | P5 | **LFO Wave #2** |
  | P6 | **LFO Wave #3** |
  | P7 | **LFO Wave #4** |

  ### I/O Expander 2 Pinout:
  | I/O Expander Pin | Parameter name |
  | :--- | :--- |
  | P0 | **Sequencer Rate #3** |
  | P1 | **Sequencer Rate #4** |
  | P2 | **LFO Wave #5** |
  | P3 | **Portamento Rate #1** |
  | P4 | **Portamento Rate #2** |
  | P5 | **Portamento Rate #3** |
  | P6 | **Portamento Rate #4** |
  | P7 | **Portamento Rate #5** |

* 3.2" 240x320 TFT display, ILI9341
  - SPI Display used for wavetable state reading

* Potentiometers
  - 20 Rotary B5K (WH148)
  - 11 Slider B10K (60mm)

* Rotary Encoder, KY-040
  - used for BPM configuration
  - button is used for preset selection

* Keyboard switches, MX linear Red
  - set up as a matrix to preserve pins

  ![Keyboard matrix configuration](Images/Keyboard_matrix_configuration.jpg "Keyboard matrix configuration")

* Toggle switches
- 3× KN3 2-way toggle switch
- 2× KN3 3-way toggle switch
- 1× MTS-102 2-way toggle switch
- 3× MTS-103 3-way toggle switch

* Basic electronic components
- Resistors
  - 1× 100 (for Display backlight)
  - 22× 1k (for LEDs)
  - 14× 10k (pull resistors)
- Capacitors
  - 5× Ceramic 100nF (for coupling)
- Diodes
  - 43× 1N4148 (for Keyboard switches)
- LEDs
  - 22× 3mm Red
- Terminal blocks
  - 45× 3-Pin 2.54mm
  - 6× 2-Pin 2.54mm

## Circuit & PCB schematics
![Main panel PCB schematics](Images/PCB_schematics-Main_panel.png "Main panel PCB schematics")
![Keyboard panel PCB schematics](Images/PCB_schematics-Keyboard_panel.png "Keyboard panel PCB schematics")

Circuit & PCB schematics have been made in KiCad 9.0 and the project is available [here.](./Design/KiCad)

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
| 14 | **Signal #1** | **LED #1 (Sequencer Rate #1)** |
| 15 | **Signal #2** | **LED #2 (Octave #4)** |
| 16 | **Signal #3** | **LED #3 (Octave #3)** |
| 17 | **Signal #4** | **LED #4 (Octave #2)** |
| 18 | **SDA** | **Audioshield** |
| 19 | **SCL** | **Audioshield** |
| 20 | **LRCLK** | **Audioshield** |
| 21 | **BCLK** | **Audioshield** |
| 22 | **Signal #5** | **LED #5 (Octave #1)** |
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
| 38 | **Signal #6** | **LED #6 (Sequencer Rate #2)** |
| 39 | **Sig (A15)** | **Mux #1** |
| 40 | **Sig (A16)** | **Mux #2** |
| 41 | **Sig (A17)** | **Mux #3** |

### 3D Model

![3D Model - front view](Images/3D_model-front_view.png "3D Model - front view")
![3D Model - side view](Images/3D_model-side_view.png "3D Model - side view")
![3D Panel](Images/3D_panel.png "3D Panel")

The 3D model has been made for laser engraving of the control panel and during the designing phase to check the real dimensions and component distances. 

## References

* Teensy 4.1 information: [link PJRC.com](https://www.pjrc.com/store/teensy41.html)
* Audio Adaptor Board for Teensy information: [link PJRC.com](https://www.pjrc.com/store/teensy3_audio.html)
