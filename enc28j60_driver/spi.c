#include <avr/io.h>
#include "spi.h"

void spi_init(void){
    // MOSI, SCK, CS output 
    DDRB |= (1 << MOSI) | (1 << SCK) | (1 << CS);

    // MISO input
    DDRB &= ~(1 << MISO);

    // CS HIGH
    DESELECT();

    PORTB |= (1 << MOSI); // MOSI HIGH

    PORTB &= ~(1 << MOSI); // MOSI LOW
			  
    PORTB &= ~(1 << SCK); // SCK LOW
    
    // Master, Mode 0, MSB first, F_CPU / 16
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);
}

void spi_transfer(uint8_t data){
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
}

uint8_t spi_read_last(void){
   return SPDR;
}
// written by an0n9m (Davit Jalagonia)
