#!/bin/bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o main.elf main.c spi.c spi.h enc28j60.c enc28j60.h uart.h uart.c arp.h arp.c icmp.h icmp.c
sleep 2
avr-objcopy -O ihex main.elf main.hex
sleep 1
avrdude -v -patmega328p -carduino -P /dev/ttyACM0 -b115200 -D -Uflash:w:main.hex:i
echo done
screen /dev/ttyACM0 115200
# written by an0n9m (Davit Jalagonia)
