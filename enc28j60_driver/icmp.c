#include <avr/io.h>
#include "enc28j60.h"
#include "uart.h"
#include <string.h>
#include "icmp.h"

void gpio_init(void)
{
    DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7);
    PORTD &= ~((1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7));
    
    DDRB |= (1<<PB0)|(1<<PB1);
    PORTB &= ~((1<<PB0)|(1<<PB1));
    // gpio setup ( HIGH = DIODE ON )
    gpio_state = 0;
}

void gpio_set(uint8_t pin, uint8_t state)
{
    if (pin < 1 || pin > 8) return;
    
    // Pin 2 - 9
    // 1 ping lights pin 2 and so on 
    if (pin <= 6) {
        uint8_t pd = pin + 1; 
        if (state) {
            PORTD |= (1 << pd);   // HIGH = LED ON
        } else {
            PORTD &= ~(1 << pd);  // LOW = LED OFF
        }
    } else {
        uint8_t pb = pin - 7;  
        if (state) {
            PORTB |= (1 << pb);   
        } else {
            PORTB &= ~(1 << pb);  
        }
    }
    
    if (state) gpio_state |= (1 << pin);
    else gpio_state &= ~(1 << pin);
}

void gpio_toggle(uint8_t pin)
{
    if (pin < 1 || pin > 8) return;
    uint8_t current = (gpio_state >> pin) & 1;
    gpio_set(pin, !current);
}

void process_ping_command(void)
{
    if (ping_count < 1 || ping_count > 8) {
        uart_puts("Bad cnt:");
        uart_putdec(ping_count);
        uart_puts("\r\n");
        return;
    }
    
    uint8_t pin = ping_count;
    
    uart_puts("Pin ");
    uart_putdec(pin + 1);  
    
    gpio_toggle(pin);
    
    if ((gpio_state >> pin) & 1) {
        uart_puts(" ON\r\n");
    } else {
        uart_puts(" OFF\r\n");
    }
}

void icmp_tick(void)
{
    if (window_active) {
        window_timer--;
        if (window_timer == 0) {
            uart_puts("Window end, pings=");
            uart_putdec(ping_count);
            uart_puts("\r\n");
            
            process_ping_command();
            
            window_active = 0;
            ping_count = 0;
        }
    }
}

void handle_icmp(uint8_t* pkt, uint16_t len, const uint8_t* my_mac, const uint8_t* my_ip)
{
    if (pkt[12] != 0x08 || pkt[13] != 0x00) return;
    if (pkt[IP_PROTO] != 0x01) return;
    if (pkt[ICMP_TYPE] != 0x08) return;
    if (memcmp(&pkt[IP_DST], my_ip, 4) != 0) return;
    
    ping_count++;
    
    if (!window_active) {
        window_active = 1;
        window_timer = WINDOW_MS / MS_PER_LOOP;
        uart_puts("Window start\r\n");
    }
    
    uart_puts("PING #");
    uart_putdec(ping_count);
    uart_puts("\r\n");
    
    // Build reply
    memcpy(&pkt[ETH_DST], &pkt[ETH_SRC], 6);
    memcpy(&pkt[ETH_SRC], my_mac, 6);
    
    uint8_t tmp[4];
    memcpy(tmp, &pkt[IP_SRC], 4);
    memcpy(&pkt[IP_SRC], &pkt[IP_DST], 4);
    memcpy(&pkt[IP_DST], tmp, 4);
    
    pkt[ICMP_TYPE] = 0x00;
    
    uint16_t csum = (pkt[ICMP_CSUM] << 8) | pkt[ICMP_CSUM + 1];
    csum += 0x0800;
    if (csum < 0x0800) csum++;
    pkt[ICMP_CSUM] = csum >> 8;
    pkt[ICMP_CSUM + 1] = csum & 0xFF;
    
    enc28j60_packet_send(pkt, len);
}
// written by an0n9m (Davit Jalagonia)
