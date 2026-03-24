# 🕹️ ESP32 Embedded Tetris Console

A fully functional, portable Tetris clone built from the ground up using an **ESP32 microcontroller**, an **I2C OLED Display**, and physical push-buttons. 

This project was developed as an academic endeavor and presented at the **Scientific Communications Session ** at the National University of Science and Technology POLITEHNICA Bucharest (UNSTPB).

## 🚀 Features
* **Custom Game Engine:** 2D/3D matrix implementations to store geometric pieces (Tetrominos) and handle rotations.
* **Real-Time Collision Detection:** Spatial algorithms to detect horizontal and vertical boundaries and block overlapping.
* **Non-Blocking Logic:** Utilizes `millis()` instead of standard delays to ensure continuous hardware polling and smooth gameplay.
* **Hardware Integrated Controls:** Responsive physical push-buttons for moving, rotating, and soft-dropping pieces.
* **Score & Highscore Tracking:** Saves and updates the high score during active sessions.

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32 Development Board
* **Display:** 0.96" OLED Display (128x64, SSD1306, I2C)
* **Controls:** 5x Push Buttons (Left, Right, Rotate, Speed Drop, Start/Restart)
* **Misc:** Breadboard and jumper wires

## 🔌 Wiring & Pinout!

| Component | ESP32 Pin |
| :--- | :--- |
| **OLED SDA** | GPIO 21 |
| **OLED SCL** | GPIO 22 |
| **Move Left Btn** | GPIO 19 |
| **Move Right Btn** | GPIO 18 |
| **Rotate Btn** | GPIO 32 |
| **Speed Drop Btn**| GPIO 33 |
| **Start/Restart Btn**| GPIO 5 |


## 💻 Software & Libraries
To compile and upload the code, you will need the **Arduino IDE** with the ESP32 board package installed.
Required libraries:
* `Wire.h` (Built-in I2C library)
* `Adafruit_GFX.h` (Core graphics library)
* `Adafruit_SSD1306.h` (Hardware-specific library for the OLED)

## 📸 Project Showcase
![WhatsApp Image 2026-03-25 at 00 58 21](https://github.com/user-attachments/assets/2809b673-de1e-4d2d-a891-23bf549b295c)


![Tetris Hardware Setup]<img width="1003" height="552" alt="circuit" src="https://github.com/user-attachments/assets/c5f3b8ff-08b4-46c5-b5c1-8e6c7879d815" />


## 📄 Documentation
For a deep dive into the architecture, collision algorithms, and hardware block diagrams, please check the [Project Documentation PDF](./Documentatie_Tetris_ESP32.pdf) included in this repository.

## 🤝 Credits & Acknowledgements
* **Authors:** Oprea George-Andrei & Simion Andrei-Alexandru
* **Coordinator:** As. dr. ing. Claudiu Tufan
* This project adapts open-source logic for matrix manipulations and translates it into a custom hardware architecture for the ESP32 ecosystem.
