# 6-Channel RF Controller

A DIY RF controller project using Arduino Nano and nRF24L01 module for radio control applications, featuring OLED display menu system.

![Arduino](https://img.shields.io/badge/Arduino-Nano-00979D)
![RF](https://img.shields.io/badge/RF-nRF24L01-red)
![Display](https://img.shields.io/badge/Display-OLED%20128x64-blue)

## 📝 Overview

This project implements a 6-channel radio control transmitter with an interactive menu system. Features include:
- Multiple control modes (Car, Airplane)
- Submenu system for different operation modes
- Real-time joystick value display
- Wireless communication using nRF24L01

## 🛠️ Hardware Requirements

### Main Components
- 1× Arduino Nano
- 2× Joystick modules
- 1× nRF24L01 + PA + LNA
- 1× Power switch (on/off button)
- 1× 128×64 OLED display (SSD1306)
- 1× 6V battery holder (4×AA)
- 4× 1.5V AA batteries

### Additional Materials
- Jumper wires
- JST connector cables
- Soldering equipment

## 📟 Pin Configuration

### Joystick Pins
```
Left Joystick:
- X-axis: A1
- Y-axis: A0
- Button: D2

Right Joystick:
- X-axis: A3
- Y-axis: A2
- Button: D3
```

### nRF24L01 Pins
```
- CE: D9
- CSN: D10
- MOSI: D11 (Hardware SPI)
- MISO: D12 (Hardware SPI)
- SCK: D13 (Hardware SPI)
```

### OLED Display Pins
```
- SDA: A4
- SCL: A5
```

## 💾 Required Libraries
```cpp
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <RF24.h>
```

## 🎮 Menu System

### Main Menu
- Car
- Airplane (Under Maintenance)

### Car Submenu
1. Free Control
   - Real-time joystick control
   - Display of joystick values
2. Line Follower
3. Random Trip

## 🔧 Features

### Free Control Mode
- Real-time transmission of joystick values
- OLED display feedback showing:
  - Left Joystick X/Y values
  - Right Joystick X/Y values
- Exit using left joystick button

### Navigation
- Use left joystick Y-axis for menu navigation
- Right button to select/enter
- Left button to return

## 📡 RF Communication

```cpp
const byte address[6] = "BTCOMM37";
```

Radio configuration:
- Power Level: RF24_PA_LOW
- Writing mode enabled
- Fixed address for communication

## 🔌 Data Structure

Joystick data packet format:
```cpp
struct {
    int lx;  // Left X-axis
    int ly;  // Left Y-axis
    int rx;  // Right X-axis
    int ry;  // Right Y-axis
} joystickData;
```

## 🎥 Assembly Guide

A detailed video tutorial showing the assembly process is available on YouTube:
[RF Controller Assembly Guide](https://www.youtube.com/watch?v=lPM2SrTrhjk)


## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
