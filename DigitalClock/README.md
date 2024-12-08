# Digital Clock Project

This project is a **complete digital clock** built using the **CVAVR** compiler and emulated using **Proteus**. The clock functionality includes setting the time, date, alarm, and using a timer and stopwatch. The system is built to run on an AVR microcontroller and includes an I2C interface to communicate with the DS1307 real-time clock (RTC) module, along with an alphanumeric LCD for display output.

## Project Modes

This digital clock has **6 modes** that are controlled by the user:

1. **Normal Mode**: Displays the current time and date on the LCD.
2. **Set Time Mode**: Allows the user to set the time by adjusting hours and minutes.
3. **Set Date Mode**: Allows the user to set the date (day, month, year).
4. **Set Alarm Mode**: Allows the user to set an alarm time.
5. **Timer Mode**: A countdown timer that the user can start or stop.
6. **Stopwatch Mode**: A stopwatch that can be started and stopped, displaying elapsed time.

## Components Used

- **CVAVR** compiler
- **Proteus** for emulation
- **DS1307 RTC Module** for timekeeping
- **LCD Display** for showing the time, date, and other modes
- **Buttons** for interaction (e.g., setting time, date, alarm)
- **AVR Microcontroller** (e.g., ATmega168)

## Emulation Screenshot

![Screenshot 2024-12-08 210325](https://github.com/user-attachments/assets/662609bc-c6e1-48f1-b787-a83675f5d7f5)


## License

This project is licensed under the GNU General Public License (GPL)
