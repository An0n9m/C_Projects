#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "enc28j60.h"
#include "uart.h"

static uint8_t current_bank = 0xFF;
static uint16_t next_packet_ptr = RXSTART_INIT;

void enc28j60_set_bank(uint8_t addr) // Setting bank
{
    uint8_t bank = (addr & BANK_MASK) >> 5;
    if (bank == current_bank)
        return;

    enc28j60_write_op(ENC_BFC, ECON1, (1 << BSEL1) | (1 << BSEL0));
    enc28j60_write_op(ENC_BFS, ECON1, bank);
    current_bank = bank;
}

uint8_t enc28j60_read_op(uint8_t op, uint8_t addr)
{
    ENABLE_CHIP();
    spi_transfer(op | (addr & ADDR_MASK));
    spi_transfer(0xFF);
    if (addr & 0x80)
        spi_transfer(0xFF);
    uint8_t result = spi_read_last();
    DISABLE_CHIP();
    return result;
}

void enc28j60_write_op(uint8_t op, uint8_t addr, uint8_t data)
{
    ENABLE_CHIP();
    spi_transfer(op | (addr & ADDR_MASK));
    spi_transfer(data);
    DISABLE_CHIP();
}

uint8_t enc28j60_read_reg(uint8_t addr)
{
    enc28j60_set_bank(addr);
    return enc28j60_read_op(ENC_READ_CTRL, addr);
}

void enc28j60_write_reg(uint8_t addr, uint8_t data)
{
    enc28j60_set_bank(addr);
    enc28j60_write_op(ENC_WRITE_CTRL, addr, data);
}

void enc28j60_init(void) // simple init
{
    ENABLE_CHIP();
    spi_transfer(ENC_SOFT_RESET);
    DISABLE_CHIP();
    _delay_ms(2);

    while (!(enc28j60_read_op(ENC_READ_CTRL, ESTAT) & ESTAT_CLKRDY)) // Checking state
        _delay_ms(1);

    current_bank = 0;
}

uint8_t enc_read_rev(void)
{
    return enc28j60_read_reg(EREVID);
}

void enc28j60_set_mac(const uint8_t mac[6])
{
    enc28j60_set_bank(MAADR0);
    enc28j60_write_reg(MAADR5, mac[0]);
    enc28j60_write_reg(MAADR4, mac[1]);
    enc28j60_write_reg(MAADR3, mac[2]);
    enc28j60_write_reg(MAADR2, mac[3]);
    enc28j60_write_reg(MAADR1, mac[4]);
    enc28j60_write_reg(MAADR0, mac[5]);
}

void enc28j60_print_mac(void)
{
    uint8_t mac[6];

    enc28j60_set_bank(MAADR0);
    mac[5] = enc28j60_read_reg(MAADR0);
    mac[4] = enc28j60_read_reg(MAADR1);
    mac[3] = enc28j60_read_reg(MAADR2);
    mac[2] = enc28j60_read_reg(MAADR3);
    mac[1] = enc28j60_read_reg(MAADR4);
    mac[0] = enc28j60_read_reg(MAADR5);

    uart_puts("MAC: ");
    for (uint8_t i = 0; i < 6; i++) {
        uart_puthex(mac[i]);
        if (i < 5) uart_putc(':');
    }
    uart_puts("\r\n");
}

void enc28j60_read_buffer(uint16_t len, uint8_t* data)
{
    ENABLE_CHIP();
    spi_transfer(ENC_READ_BUF);
    while (len--) {
        spi_transfer(0x00);
        *data++ = spi_read_last();
    }
    DISABLE_CHIP();
}

void enc28j60_write_buffer(uint16_t len, const uint8_t* data)
{
    ENABLE_CHIP();
    spi_transfer(ENC_WRITE_BUF);
    while (len--) {
        spi_transfer(*data++);
    }
    DISABLE_CHIP();
}

void enc28j60_write_phy(uint8_t addr, uint16_t data)
{
    enc28j60_write_reg(MIREGADR, addr);
    enc28j60_write_reg(MIWRL, data & 0xFF);
    enc28j60_write_reg(MIWRH, data >> 8);
    
    while (enc28j60_read_reg(MISTAT) & 0x01)
        _delay_us(10);
}

void enc28j60_full_init(const uint8_t* mac)
{
    enc28j60_init();

    // Setup RX buffer
    enc28j60_write_reg(ERXSTL, RXSTART_INIT & 0xFF);
    enc28j60_write_reg(ERXSTH, RXSTART_INIT >> 8);
    enc28j60_write_reg(ERXNDL, RXSTOP_INIT & 0xFF);
    enc28j60_write_reg(ERXNDH, RXSTOP_INIT >> 8);
    
    // ERXRDPTL setup
    enc28j60_write_reg(ERXRDPTL, RXSTOP_INIT & 0xFF);
    enc28j60_write_reg(ERXRDPTH, RXSTART_INIT >> 8);

    // Setup TX buffer
    enc28j60_write_reg(ETXSTL, TXSTART_INIT & 0xFF);
    enc28j60_write_reg(ETXSTH, TXSTART_INIT >> 8);
    enc28j60_write_reg(ETXNDL, TXSTOP_INIT & 0xFF);
    enc28j60_write_reg(ETXNDH, TXSTOP_INIT >> 8);

    // Unicast, Broadcast, and CRC checking
    enc28j60_set_bank(ERXFCON);
    enc28j60_write_reg(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_BCEN);
    // MAC receive
    enc28j60_write_reg(MACON1, MACON1_MARXEN);

    // Configure MAC: padding, CRC, frame length checking
    enc28j60_write_op(ENC_BFS, MACON3, 
                      MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN);

    // Configure inter-packet gap
    enc28j60_write_reg(MAIPGL, 0x12);
    enc28j60_write_reg(MAIPGH, 0x0C);
    enc28j60_write_reg(MABBIPG, 0x12);
    
    // Set maximum frame length
    enc28j60_write_reg(MAMXFLL, MAX_FRAMELEN & 0xFF);
    enc28j60_write_reg(MAMXFLH, MAX_FRAMELEN >> 8);

    // Set MAC address
    enc28j60_set_mac(mac);

    // PHY configuration: disable loopback
    enc28j60_write_phy(PHCON2, PHCON2_HDLDIS);
    
    // LED configuration
    enc28j60_write_phy(PHCON1, 0x0100);  // PDPXMD = full duplex
    _delay_ms(50);  // Give PHY time to settle
    
    enc28j60_write_phy(PHLCON, 0x0476);

    // Enable interrupts and packet reception
    enc28j60_set_bank(ECON1);
    enc28j60_write_op(ENC_BFS, EIE, EIE_INTIE | EIE_PKTIE);
    enc28j60_write_op(ENC_BFS, ECON1, ECON1_RXEN);
    
    uart_puts("ENC28J60 init complete\r\n");
}

uint16_t enc28j60_get_rxrdpt(void)
{
    return enc28j60_read_reg(ERXRDPTL) | (enc28j60_read_reg(ERXRDPTH) << 8);
}

void enc28j60_set_rxrdpt(uint16_t ptr)
{
    enc28j60_write_reg(ERDPTL, ptr & 0xFF);
    enc28j60_write_reg(ERDPTH, ptr >> 8);
}

uint8_t enc28j60_packet_pending(void)
{
    enc28j60_set_bank(ECON1);
    return enc28j60_read_reg(EPKTCNT);
}

uint16_t enc28j60_packet_receive(uint8_t* pkt, uint16_t max_len)
{
    uint16_t len = 0;
    
    if (enc28j60_read_reg(EPKTCNT) == 0)
        return 0;

    // Set read pointer to next packet
    enc28j60_write_reg(ERDPTL, next_packet_ptr & 0xFF);
    enc28j60_write_reg(ERDPTH, next_packet_ptr >> 8);

    // Read 6-byte header
    uint8_t header[6];
    enc28j60_read_buffer(6, header);

    // Parse header
    uint16_t next_ptr = header[0] | (header[1] << 8);
    uint16_t rx_len = header[2] | (header[3] << 8);
    uint16_t rx_status = header[4] | (header[5] << 8);

    next_packet_ptr = next_ptr;
    
    // Remove CRC
    len = rx_len - 4;
    
    // Sanity checks
    if (len > max_len - 1)
        len = max_len - 1;
    
    if ((rx_status & 0x80) == 0)
        len = 0;
    else
        enc28j60_read_buffer(len, pkt);

    // Update ERXRDPT (with wraparound handling)
    if (next_packet_ptr - 1 > RXSTOP_INIT)
        enc28j60_write_reg(ERXRDPTL, RXSTOP_INIT & 0xFF),
        enc28j60_write_reg(ERXRDPTH, RXSTOP_INIT >> 8);
    else
        enc28j60_write_reg(ERXRDPTL, (next_packet_ptr - 1) & 0xFF),
        enc28j60_write_reg(ERXRDPTH, (next_packet_ptr - 1) >> 8);

    // Decrement packet counter
    enc28j60_write_op(ENC_BFS, ECON2, ECON2_PKTDEC);

    return len;
}

  

void uart_hexdump(const uint8_t* data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++) {
        uart_puthex(data[i]);
        uart_putc(' ');
        if ((i + 1) % 16 == 0)
            uart_puts("\r\n");
    }
    if (len % 16 != 0)
        uart_puts("\r\n");
}
void enc28j60_packet_send(const uint8_t* pkt, uint16_t len)
{
    // Transmission control
    while (enc28j60_read_op(ENC_READ_CTRL, ECON1) & 0x08)
        _delay_us(10);

    // write pointer to TX buffer
    enc28j60_write_reg(EWRPTL, TXSTART_INIT & 0xFF);
    enc28j60_write_reg(EWRPTH, TXSTART_INIT >> 8);

    // Set TX end pointer
    enc28j60_write_reg(ETXNDL, (TXSTART_INIT + len) & 0xFF);
    enc28j60_write_reg(ETXNDH, (TXSTART_INIT + len) >> 8);

    // Write per-packet control byte (0x00 = use MACON3 settings)
    enc28j60_write_op(ENC_WRITE_BUF, 0, 0x00);

    // Write packet data
    enc28j60_write_buffer(len, pkt);

    // Start transmission
    enc28j60_write_op(ENC_BFS, ECON1, 0x08); // ECON1_TXRTS
}

uint16_t enc28j60_read_phy(uint8_t addr)
{
    enc28j60_write_reg(MIREGADR, addr);
    enc28j60_write_reg(MICMD, 0x01);  // Start read
    _delay_us(15);
    while (enc28j60_read_reg(MISTAT) & 0x01)
        _delay_us(10);
    enc28j60_write_reg(MICMD, 0x00);  // Stop read
    return enc28j60_read_reg(MIRDL) | (enc28j60_read_reg(MIRDH) << 8);
}
// written by an0n9m (Davit Jalagonia)
