#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi_init(void);
void spi_transfer(uint8_t data);
uint8_t spi_read_last(void);

#define SELECT()    (PORTB &= ~(1 << PB2))
#define DESELECT()  (PORTB |=  (1 << PB2))
#define CS   PB2
#define MOSI PB3
#define MISO PB4
#define SCK  PB5


#endif

// written by an0n9m (Davit Jalagonia)
