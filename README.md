# ESP32 Tetris Console 

This is a custom Tetris game I built using an ESP32 microcontroller and a small OLED display. It was initially developed as a university project for the Scientific Communications Session (SSCF) at UNSTPB, but it turned out to be a great way to practice C++ and hardware integration.

## How it works
Instead of relying on heavy game libraries, the game logic is handled directly in the code:
* **The Pieces:** I used multidimensional arrays to store the Tetromino shapes and their rotation states.
* **Collisions:** The engine always checks the grid array before moving or rotating a piece, making sure it doesn't clip through walls or other blocks.
* **Smooth controls:** I completely avoided using `delay()`. By using `millis()` for the timing logic, the ESP32 constantly reads the button inputs without freezing the game.

## What I used
* **ESP32** Development Board
* **0.96" OLED Display** (I2C / SSD1306)
* 5x Push Buttons (Left, Right, Rotate, Fast Drop, Restart)
* Breadboard and some jumper wires

## Wiring Setup
The buttons are configured with the ESP32's internal pull-up resistors (`INPUT_PULLUP`), so they just need to be wired directly to GND.

| Component | ESP32 Pin |
| :--- | :--- |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| Move Left | GPIO 19 |
| Move Right | GPIO 18 |
| Rotate | GPIO 32 |
| Speed Drop | GPIO 33 |
| Restart | GPIO 5 |

## Project Showcase
![WhatsApp Image 2026-03-25 at 00 58 21](https://github.com/user-attachments/assets/51adfb72-488f-494b-844b-9a72f045ee6b)

<img width="1003" height="552" alt="circuit" src="https://github.com/user-attachments/assets/1bc109c0-5c35-4a53-88c0-ae3adc9f090a" />

## Want to run it?
1. Open the code in the **Arduino IDE**.
2. Make sure you have the ESP32 board package installed.
3. Download the `Adafruit GFX` and `Adafruit SSD1306` libraries from the Library Manager.
4. Compile, upload, and play!

---
*Note: For more technical details and block diagrams, feel free to check out the PDF documentation included in this repository. 
Worked on this alongside my colleague, Andrei-Alexandru Simion.*
