#include <avr/io.h>
#include "uart.h"

void uart_init(uint32_t baud)
{
    UCSR0A = (1 << U2X0);   // 2x speed mode

    uint16_t ubrr = (F_CPU / 8 / baud) - 1;
    UBRR0H = (ubrr >> 8);
    UBRR0L = ubrr;

    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putc(char c)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_puts(const char *s)
{
    while (*s)
        uart_putc(*s++);
}

void uart_puthex(uint8_t v)
{
    char hex[] = "0123456789ABCDEF";
    uart_putc(hex[v >> 4]);
    uart_putc(hex[v & 0x0F]);
}
void uart_puthex16(uint16_t val)
{
    uart_puthex(val >> 8);
    uart_puthex(val & 0xFF);
}
void uart_putdec(uint8_t val)
{
    if (val >= 100) {
        uart_putc('0' + (val / 100));
        val %= 100;
    }
    if (val >= 10) {
        uart_putc('0' + (val / 10));
        val %= 10;
    }
    uart_putc('0' + val);
}

void uart_putdec16(uint16_t val)
{
    uint8_t started = 0;
    uint16_t div = 10000;

    while (div > 0) {
        uint8_t digit = val / div;
        if (digit || started || div == 1) {
            uart_putc('0' + digit);
            started = 1;
        }
        val %= div;
        div /= 10;
    }
}
// written by an0n9m (Davit Jalagonia)
