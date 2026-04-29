# GladiaterAllegience

A custom wireless transmitter/receiver system built around the **ESP32-C3** and **nRF24L01** 2.4 GHz radio module, with fully custom PCBs designed in KiCad.

---

## Overview

GladiaterAllegience is a hardware project consisting of two paired PCB modules:

- **TransmitterModule** — a handheld controller with buttons and potentiometers that sends commands over nRF24L01 RF
- **ReceiverModule** — the on-device board that receives those commands and drives outputs (LEDs, servos, etc.)

Firmware runs on the ESP32-C3 and is written in Arduino/C++.

---

## Repository Structure

```
GladiaterAllegience/
│
├── TransmitterModule/                  # KiCad PCB project — handheld transmitter (current)
│   ├── sgh24_TransmitterModule.kicad_pro
│   ├── sgh24_TransmitterModule.kicad_sch
│   ├── sgh24_TransmitterModule.kicad_pcb
│   ├── sgh24_TransmitterModule.step    # 3D model export
│   └── Transmitter Output Files/       # Gerbers + drill files ready for fabrication
│
├── ReceiverModule_V1/                  # KiCad PCB project — receiver board (v1)
│   ├── ReceiverModule.kicad_pro
│   ├── ReceiverModule.kicad_sch
│   └── ReceiverModule.kicad_pcb
│
├── ReceiverModule_V2/                  # KiCad PCB project — receiver board (v2, current)
│   ├── ReceiverModule_V2.kicad_pro
│   ├── ReceiverModule_V2.kicad_sch
│   └── ReceiverModule_V2.kicad_pcb
│
├── TranceiverV1_Circuit/               # KiCad PCB project — early combined transceiver board
│   ├── TranceiverV1.kicad_pro
│   ├── TranceiverV1.kicad_sch
│   └── TranceiverV1.kicad_pcb
│
├── ESP32C3_Transmitter_Receiver_Arduino/
│   ├── Transmitter_V2/
│   │   └── Gladiator_Transmitter_V2/   # ESP32-C3 transmitter firmware
│   └── Receiver_V2/
│       └── Gladiator_Receiver_V2/      # ESP32-C3 receiver firmware
│
├── ESP32C3_Servo_Arduino/
│   └── Gladiator_Allegiance_Servo/     # ESP32-C3 servo control firmware
│
├── ESP32C3_Servo_Receiver_Final_Arduino/
│   └── ESP32C3_Final_Receiver/         # Final receiver firmware with servo output
│
├── Components/                         # KiCad custom component library
│   ├── NRF24L01_/                      # nRF24L01 symbol + footprint + 3D model
│   ├── 102010388/                      # Seeed XIAO ESP32-C3 symbol + footprint
│   ├── ATE1D-2M3-10-Z/                # Relay symbol + footprint + 3D model
│   └── TS02_66_50_BK_100_LCR_D/      # Tactile button symbol + footprint
│
├── ServoArduino_V1/
│   └── servo_sweep_test/               # Early servo sweep test (Arduino Uno)
│
└── Arduino_NRF24_Test/
    └── ReceiverModule/                 # Early nRF24L01 receiver proof-of-concept
```

---

## Hardware

| Module | MCU | Radio | Key components |
|---|---|---|---|
| Transmitter | ESP32-C3 (Seeed XIAO) | nRF24L01 | Buttons, potentiometers |
| Receiver V1 | ESP32-C3 (Seeed XIAO) | nRF24L01 | LED outputs |
| Receiver V2 | ESP32-C3 (Seeed XIAO) | nRF24L01 | LED outputs + servo |

PCBs are designed in **KiCad 8**. Fabrication-ready Gerbers for the transmitter are in `TransmitterModule/Transmitter Output Files/`.

---

## Firmware

### Transmitter (`ESP32C3_Transmitter_Receiver_Arduino/Transmitter_V2/Gladiator_Transmitter_V2/`)

Reads a button (active LOW) and transmits over nRF24L01 when pressed. Blinks an LED on successful TX.

**Dependencies:** `RF24` library

### Receiver (`ESP32C3_Transmitter_Receiver_Arduino/Receiver_V2/Gladiator_Receiver_V2/`)

Listens on the nRF24L01 address and drives LED outputs. Uses a button-triggered state machine to control eye and nose LED brightness/blink rate from potentiometer readings.

**Dependencies:** `RF24` library

### Servo + Final Receiver (`ESP32C3_Servo_Arduino/` and `ESP32C3_Servo_Receiver_Final_Arduino/`)

Servo control firmware and the final integrated receiver that combines RF reception with servo output.

**Dependencies:** `RF24` library, `Servo` library

### Flashing

1. Install the [ESP32 board package](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) in Arduino IDE
2. Select board: **XIAO_ESP32C3**
3. Install the `RF24` library via Library Manager
4. Open the `.ino` file and upload

---

## KiCad Setup

Custom component libraries are stored in `Components/`. To use them:

1. Open KiCad → Preferences → Manage Symbol Libraries → Add the `.kicad_sym` files
2. Open KiCad → Preferences → Manage Footprint Libraries → Add the `.kicad_mod` files
