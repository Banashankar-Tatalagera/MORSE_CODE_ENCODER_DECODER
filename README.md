# 🔠 Arduino Morse Code Translator


[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![Platform](https://img.shields.io/badge/platform-Arduino-blue)]()
[![Components](https://img.shields.io/badge/components-IR%20Sensor%2C%20LCD%2C%20Buzzer-lightgrey)]()
[![Made With](https://img.shields.io/badge/made%20with-Arduino%20IDE-blue)]()

> A multi-mode Morse code translator using Arduino, capable of decoding input from push button and IR sensor, and encoding via Serial. Real-time output via LCD, LED, and buzzer.

---

## 🚀 Features

- 🔘 **Push Button Decode**: Short/long press detection.
- 📡 **IR Sensor Decode**: Detects short/long gestures.
- 💻 **Serial Encode Mode**: Inputs typed text and plays Morse using LED + buzzer.
- 🔄 **System Reset**: Clears data, resets to default.
- 📟 **LCD Output**: Morse and decoded messages in real-time.
- 🔔 **LED + Buzzer Feedback**: Audio/visual signal during encoding/decoding.
- 🔄 **Mode Switching**: Single button toggles modes with feedback.

---

## 🛠️ Hardware Required

| Component          | Quantity  |
| ------------------ | --------- |
| Arduino Uno        | 1         |
| IR Sensor Module   | 1         |
| Push Button        | 1         |
| I2C 16x2 LCD       | 1         |
| Buzzer (Active)    | 1         |
| LED                | 1         |
| Breadboard & Wires | As needed |

---

## 🔍 How It Works

### ⌨️ Push Button Mode
- Press < 400ms → Dot `·`
- Press ≥ 400ms → Dash `–`
- Idle > 2s → Decode letter
- Idle > 3s → Space (new word)

### ✋ IR Sensor Mode
- Hand < 600ms → Dot `·`
- Hand ≥ 600ms → Dash `–`
- Idle > 1.5s → Decode letter
- Idle > 3s → Space

### 💻 Serial Encode Mode
- Send text via Serial Monitor
- Morse output via buzzer & LED
- LCD shows both Morse and decoded text

### 🔁 Reset Mode
- Clears all variables and resets display.

---

## 📦 Setup Instructions

1. **Wire Your Components** (see schematic)
2. **Open Arduino IDE**
3. **Install Library**: `LiquidCrystal_I2C`
4. **Upload Sketch**: Paste and flash the `.ino` file
5. **Open Serial Monitor (9600 baud)** for serial input mode

---

## 🔄 Switching Modes

| Mode               | Beep Feedback | LCD Message           |
| ------------------ | ------------- | --------------------- |
| Push Button Decode | 1 Beep        | "Mode: Button Decode" |
| IR Sensor Decode   | 2 Beeps       | "Mode: IR Decode"     |
| Serial Encode      | 3 Beeps       | "Mode: Serial Encode" |
| Reset              | 3 Long Beeps  | "System Reset!"       |

Hold push button > 2s to switch modes cyclically.

---



## 📚 References

- [Arduino Documentation](https://www.arduino.cc/en/Guide)
- [LiquidCrystal_I2C Library](https://github.com/johnrickman/LiquidCrystal_I2C)
- [Morse Code Wiki](https://en.wikipedia.org/wiki/Morse_code)

---






## 🙌 Contributors

Thanks to all contributors who made this project possible:

| Name                   | GitHub Profile                                                        |
|------------------------|------------------------------------------------------------------------|
| Banashankar Tatalagera | [@Banashankar-Tatalagera](https://github.com/Banashankar-Tatalagera) |
|Bharath K C             | [@bharathkc05](https://github.com/bharathkc05)                            |
| Basanagouda Patil               | [@BASANAGOUDA6174 ](https://github.com/BASANAGOUDA6174)                        |

