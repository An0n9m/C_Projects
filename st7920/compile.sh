#!/bin/bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o main.elf main.c st7920lib.h st7920lib.c 
sleep 2
avr-objcopy -O ihex main.elf main.hex
sleep 1
avrdude -v -patmega328p -carduino -P /dev/ttyACM0 -b115200 -D -Uflash:w:main.hex:i
echo done
# written by an0n9m (Davit Jalagonia)
