# ST7920 LCD Driver for ATmega328P

This is a driver library for the ST7920 LCD controller, designed for the Arduino Uno (ATmega328P) but adaptable to other microcontrollers with minor adjustments. The library supports both text (DDRAM) and graphic (GDRAM) modes, enabling text display and 128x64 pixel graphics. It includes a framebuffer for pixel manipulation and provides four test cases to demonstrate functionality, including a simple Pong simulation.

## Features
- Initialize and control the ST7920 LCD in text and graphic modes.
- Framebuffer-based graphics with pixel-level control (set/clear pixels).
- Support for 128x64 pixel display in graphic mode.
- Four test cases:
  1. **DDRAM_test**: Displays scrolling text ("PLAYER1" and "PLAYER2") in text mode.
  2. **GDRAM_test**: Draws a moving horizontal line in graphic mode.
  3. **DDRAM_and_GDRAM_test**: Combines text ("DAVID" and "BOB") with a Pong simulation in graphic mode.
  4. **pong_test**: A simple Pong game simulation using GDRAM.
- Includes a `compile.sh` script for easy building and uploading to Arduino Uno.

## Hardware Setup
Connect the ST7920 LCD to your Arduino Uno as follows (modify pin definitions in `st7920lib.h` if needed):
- **RS (Register Select)**: PB0 (Arduino pin 8)
- **RW (Read/Write)**: PB1 (Arduino pin 9)
- **E (Enable)**: PB2 (Arduino pin 10)
- **RE (Reset)**: PB3 (Arduino pin 11)
- **Data Pins (DB0-DB7)**: PORTD (Arduino pins 0-7)
- **Power**: VCC to 5V, GND to GND, and ensure proper connections for backlight (if used).

## Installation
1. Clone this repository:
   ```bash
   git clone https://github.com/<your-username>/st7920-driver.git
   ```
2. Ensure you have the AVR-GCC toolchain and `avrdude` installed (e.g., via Arduino IDE or standalone).
3. Connect your Arduino Uno to your computer via USB.

## Compilation and Upload
The provided `compile.sh` script automates building and uploading:
```bash
chmod +x compile.sh
./compile.sh
```
The script:
- Compiles the code using `avr-gcc` for ATmega328P.
- Converts the output to a `.hex` file using `avr-objcopy`.
- Uploads the `.hex` file to the Arduino Uno via `/dev/ttyACM0` using `avrdude`.

Ensure `/dev/ttyACM0` matches your Arduino's serial port (modify the script if needed).

## Usage
1. Open `main.c` and uncomment one of the test case functions in `main()` to run it:
   ```c
   DDRAM_test();          // Testcase #1: Text display
   // GDRAM_test();       // Testcase #2: Graphic line
   // DDRAM_and_GDRAM_test(); // Testcase #3: Text + Pong
   // pong_test();        // Testcase #4: Pong simulation
   ```
2. Run `./compile.sh` to build and upload the code.
3. The selected test case will run on the LCD.

## License
This project is licensed under the **GNU General Public License v3.0**. You are free to use, modify, and distribute this software, provided any derivative works are also licensed under the GPL v3. See the `COPYING` file for the full license text.

For more details, visit [https://www.gnu.org/licenses/gpl-3.0.html](https://www.gnu.org/licenses/gpl-3.0.html).

## Author
Written by Davit Jalagonia (an0n9m).

## Contributing
Contributions are welcome! Please submit a pull request or open an issue on GitHub.