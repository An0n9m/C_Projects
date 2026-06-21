#include "uart.h"
#include "spi.h"
#include "enc28j60.h"
#include "arp.h"
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include "icmp.h"

// Mac and IP setup
uint8_t my_mac[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
uint8_t my_ip[4] = {192, 168, 0, 57};

uint8_t dbg_read(uint8_t bank, uint8_t addr)
{
    enc28j60_write_op(ENC_BFC, ECON1, 0x03);
    enc28j60_write_op(ENC_BFS, ECON1, bank & 0x03);
    return enc28j60_read_op(ENC_READ_CTRL, addr & 0x1F);
}

int main(void)
{
    uart_init(115200);
    _delay_ms(100);
    
    uart_puts("\r\n== ENC DEBUG ==\r\n");
    
    spi_init();
    uart_puts("SPI OK\r\n");
    
    // Checkup
    uint8_t rev = dbg_read(3, 0x12);
    uart_puts("REV:0x");
    uart_puthex(rev);
    uart_puts("\r\n");
    if (rev == 0x00 || rev == 0xFF) {
        uart_puts("CHIP FAIL!\r\n");
        while(1);
    }
    
    // Init
    enc28j60_full_init(my_mac);
    uart_puts("Init done\r\n");
    
    uart_puts("ECON1:0x");
    uart_puthex(dbg_read(0, 0x1F));
    uart_puts("\r\n");
    
    uart_puts("ERXFCON:0x");
    uart_puthex(dbg_read(1, 0x18));
    uart_puts("\r\n");
    
    uart_puts("EPKTCNT:0x");
    uart_puthex(dbg_read(1, 0x19));
    uart_puts("\r\n");
    
    enc28j60_write_op(ENC_BFC, ECON1, 0x03);
    enc28j60_write_op(ENC_BFS, ECON1, 0x02);
    uart_puts("MACON1:0x");
    uart_puthex(enc28j60_read_op(ENC_READ_CTRL, 0x00 | 0x80));
    uart_puts("\r\n");
    uart_puts("MACON3:0x");
    uart_puthex(enc28j60_read_op(ENC_READ_CTRL, 0x02 | 0x80));
    uart_puts("\r\n");
    
    // PHY
    uint16_t phstat2 = enc28j60_read_phy(PHSTAT2);
    uart_puts("PHSTAT2:0x");
    uart_puthex16(phstat2);
    uart_puts(phstat2 & 0x0400 ? " LINK\r\n" : " NOLINK\r\n");
    
    // ERXWRPT - hardware write pointer
    uint16_t wrpt1 = dbg_read(0, 0x0E) | (dbg_read(0, 0x0F) << 8);
    uart_puts("ERXWRPT:0x");
    uart_puthex16(wrpt1);
    uart_puts("\r\nWait 3s...\r\n");
    _delay_ms(3000);
    
    uint16_t wrpt2 = dbg_read(0, 0x0E) | (dbg_read(0, 0x0F) << 8);
    uart_puts("ERXWRPT:0x");
    uart_puthex16(wrpt2);
    if (wrpt2 != wrpt1) {
        uart_puts(" CHANGED!\r\n");
    } else {
        uart_puts(" SAME\r\n");
    }
    
    uart_puts("\r\nMAC:");
    print_mac(my_mac);
    uart_puts("\r\nIP:");
    print_ip(my_ip);
    uart_puts("\r\n\r\n");

    uint8_t pkt[400];
    uint16_t cnt = 0;
    uint16_t last_wrpt = wrpt2;
while (1) {
    uint8_t pktcnt = dbg_read(1, 0x19);
    
    if (pktcnt > 0) {
        uint16_t len = enc28j60_packet_receive(pkt, sizeof(pkt));
        
        if (len > 0) {
            // Check if ARP (ethertype at bytes 12-13)
            if (pkt[12] == 0x08 && pkt[13] == 0x06) {
                uart_puts("ARP! tgt=");
                // Target IP is at offset 38 (14 eth + 24 arp offset)
                uart_putdec(pkt[38]);
                uart_putc('.');
                uart_putdec(pkt[39]);
                uart_putc('.');
                uart_putdec(pkt[40]);
                uart_putc('.');
                uart_putdec(pkt[41]);
                uart_puts("\r\n");
                
                // Check if it's for us
                if (pkt[38] == my_ip[0] && pkt[39] == my_ip[1] &&
                    pkt[40] == my_ip[2] && pkt[41] == my_ip[3]) {
                    uart_puts("FOR US! Replying...\r\n");
                    send_arp_reply(my_mac, my_ip, &pkt[22], &pkt[28]);
                }
            }
       		handle_icmp(pkt, len, my_mac, my_ip);
	}
    }
    icmp_tick();  
    _delay_ms(500);
}
}
// written by an0n9m (Davit Jalagonia)
