# Speden Spelit – Reaction Game 🎮

**Authors:**  
Jussi Mitteli, Jere Peltola, Elmo Lehtoniemi, Antti Buller  

---

## 📘 Project Description

The goal of this project was to recreate the **legendary Speden Spelit reaction game** using Arduino.  
The game tests the player's **reaction and memory speed**:  
- LEDs flash in a specific sequence.  
- The player's task is to press the correct button as quickly as possible.  
- Each correct press gives one point, and a wrong press ends the game.  
- After every 10 correct presses, the game speed increases by 10%.  

This project was an exercise in developing a microcontroller-based application, handling interrupts, timers, and modular programming.  

---

## 🧩 System Overview

The game consists of four main modules:  

| Module | Files | Description |
|---------|--------|-------------|
| **Buttons** | `buttons.h`, `buttons.cpp` | Reads the state of four push buttons using interrupts and implements software debouncing. |
| **Leds** | `leds.h`, `leds.cpp` | Controls LED blinking. Only one LED is on at a time. Includes light shows `show1()` and `show2()`. |
| **Display** | `display.h`, `display.cpp` | Controls two 7-segment displays via two 74HC595 shift registers. Displays the player’s score. |
| **Gamelogic** | `nopeuspeli.ino`, `nopeuspeli.h` | The main program controlling game logic and scorekeeping. |

---

## ⚙️ Components and Tools  

**Hardware Components:**  
- Arduino Uno  
- 4x LED buttons  
- 2x 7-segment displays  
- 2x 74HC595 shift registers  
- 1kΩ and 10kΩ resistors  
- Jumper wires and breadboard  

**Software Tools:**  
- Visual Studio Code / Arduino IDE  
- Teams & Discord  
- Autodesk Fusion (for circuit schematic design)  

---

## 🔄 Program Flow

1. **Initialization:**  
   - Pins, interrupts, display, and timer are initialized.  
   - A LED blinks to indicate standby mode.  

2. **Game Start:**  
   - The player presses the start button.  
   - LEDs light up, and the display resets to zero.  

3. **Gameplay:**  
   - The timer controls LED flashing.  
   - The player must press the button corresponding to the lit LED.  

4. **Scoring:**  
   - A correct press adds one point and updates the display.  
   - After every 10 correct presses, the speed increases by 10%.  

5. **Error / Game Over:**  
   - A wrong press ends the game.  
   - All LEDs flash, and the final score is displayed.  

6. **Reset:**  
   - The game can be restarted by pressing a button again.  

---

## 🕹️ User Instructions

1. Connect all components according to the wiring diagram.  
2. Open the project in Arduino IDE and upload it to the Arduino Uno microcontroller.  
3. Wait until the indicator LED starts blinking – the game is ready.  
4. Hold the **start button** for 2 seconds to begin.  
5. Follow the LEDs and press the corresponding button.  
6. The score is displayed in real time on the 7-segment display.  
7. A wrong press ends the game and shows the final score.  

![Wiring Diagram](Kytkentäkaavio.png)

---

## 🧠 Learning Objectives  

- Handling microcontroller interrupts, timers, and I/O pins  
- Programming a 74HC595 shift register manually (without libraries)  
- Practicing structured modular programming  
- Teamwork, documentation, and version control  
- Designing game logic and error handling  

---

## 📸 Images and Attachments  

- `Kytkentäkaavio.png` – wiring diagram  
- `Flowchart_V2.pdf` – game flowchart  

---

## 🏁 Final Outcome  

The result of the project is a fully functional **Speden Spelit reaction game** that includes all core features.  

---
