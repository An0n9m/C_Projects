#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint32_t baud);
void uart_putc(char c);
void uart_puts(const char *s);
void uart_puthex(uint8_t v);
void uart_puthex16(uint16_t val);
void uart_putdec(uint8_t val);
void uart_putdec16(uint16_t val);
#endif
// written by an0n9m (Davit Jalagonia)
