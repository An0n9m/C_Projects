#ifndef ENC28J60_H
#define ENC28J60_H

#include <stdint.h>

#define ENC_PKT_READY   0x01

// ERXFCON bits
#define ERXFCON_UCEN    0x80
#define ERXFCON_BCEN    0x01
#define ERXFCON_CRCEN   0x20
#define ERXFCON_PMEN    0x10
#define ERXFCON_MPEN    0x08

// MACON1 bits
#define MACON1_MARXEN   0x01

// MACON3 bits 
#define MACON3_PADCFG0  0x20  
#define MACON3_TXCRCEN  0x10  
#define MACON3_FRMLNEN  0x02  // Frame length checking

// EIE bits
#define EIE_INTIE       0x80
#define EIE_PKTIE       0x40

// ECON bits
#define ECON1_RXEN      0x04
#define ECON2           0x1E
#define ECON2_PKTDEC    0x40

// EPKTCNT (bank 1)
#define EPKTCNT         0x19 | 0x20

// PHY registers
#define PHCON2_HDLDIS   0x0100

// Opcodes
#define ENC_BIT_FIELD_SET   0x80
#define ENC_BIT_FIELD_CLR   0xA0
#define ENC_READ_CTRL       0x00
#define ENC_WRITE_CTRL      0x40
#define ENC_BFS             0x80
#define ENC_BFC             0xA0
#define ENC_SOFT_RESET      0xFF
#define ENC_READ_BUF        0x3A
#define ENC_WRITE_BUF       0x7A

// Control registers (Bank 0)
#define ERDPTL   0x00
#define ERDPTH   0x01
#define EWRPTL   0x02
#define EWRPTH   0x03
#define ETXSTL   0x04
#define ETXSTH   0x05
#define ETXNDL   0x06
#define ETXNDH   0x07
#define ERXSTL   0x08
#define ERXSTH   0x09
#define ERXNDL   0x0A
#define ERXNDH   0x0B
#define ERXRDPTL 0x0C
#define ERXRDPTH 0x0D
#define ERXFCON  (0x18|0x20)
#define EIE      0x1B
#define EIR      0x1C
#define ESTAT    0x1D
#define ECON2    0x1E
#define ECON1    0x1F

// Bank 1 - MAC control registers
#define MACON1   0x00 | 0x40 | 0x80
#define MACON3   0x02 | 0x40 | 0x80
#define MACON4   0x03 | 0x40 | 0x80
#define MABBIPG  0x04 | 0x40 | 0x80
#define MAIPGL   0x06 | 0x40 | 0x80
#define MAIPGH   0x07 | 0x40 | 0x80
#define MAMXFLL  0x0A | 0x40 | 0x80
#define MAMXFLH  0x0B | 0x40 | 0x80

// Bank 1 - Pattern match registers
#define EPMM0    0x08 | 0x20
#define EPMM1    0x09 | 0x20
#define EPMM2    0x0A | 0x20
#define EPMM3    0x0B | 0x20
#define EPMCS    0x10 | 0x20
#define EPMCSH   0x11 | 0x20

// Bank 2 - MAC registers
#define MAADR1   (0x00|0x60|0x80)
#define MAADR0   (0x01|0x60|0x80)
#define MAADR3   (0x02|0x60|0x80)
#define MAADR2   (0x03|0x60|0x80)
#define MAADR5   (0x04|0x60|0x80)
#define MAADR4   (0x05|0x60|0x80)
#define MICMD    (0x12 | 0x40 | 0x80)  // Bank 2, MAC register
#define MIRDL    (0x18 | 0x40 | 0x80)
#define MIRDH    (0x19 | 0x40 | 0x80)
#define PHSTAT1  0x01
#define PHSTAT2  0x11

// Bank 2 - MII registers
#define MIREGADR 0x14 | 0x40 | 0x80
#define MIWRL    0x16 | 0x40 | 0x80
#define MIWRH    0x17 | 0x40 | 0x80

// Bank 3
#define MISTAT   0x0A | 0x60 | 0x80
#define EREVID   0x12 | 0x60

// PHY registers
#define PHCON1   0x00
#define PHCON2   0x10
#define PHLCON   0x14

// Buffer sizes
#define RXSTART_INIT    0x0000
#define RXSTOP_INIT     0x0BFF
#define TXSTART_INIT    0x0C00
#define TXSTOP_INIT     0x11FF
#define MAX_FRAMELEN    1518

// Masks
#define ADDR_MASK       0x1F
#define BANK_MASK       0x60

// ESTAT bits
#define ESTAT_CLKRDY    0x01

// ECON1 bits
#define BSEL0           0
#define BSEL1           1

#define ENC_CS_DELAY()  _delay_us(1)

#define ENABLE_CHIP()  { SELECT(); _delay_us(1); }
#define DISABLE_CHIP() { DESELECT();}

// Func declarations
void enc28j60_init(void);
uint8_t enc28j60_read_rev(void);
uint8_t enc_read_rev(void);

uint8_t enc28j60_read_op(uint8_t op, uint8_t addr);
void enc28j60_write_op(uint8_t op, uint8_t addr, uint8_t data);

uint8_t enc28j60_read_reg(uint8_t addr);
void enc28j60_write_reg(uint8_t addr, uint8_t data);
void enc28j60_print_mac(void);
void enc28j60_set_mac(const uint8_t mac[6]);
void enc28j60_read_buffer(uint16_t len, uint8_t* data);
void enc28j60_write_buffer(uint16_t len, const uint8_t* data);
void enc28j60_full_init(const uint8_t* mac);
uint16_t enc28j60_get_rxrdpt(void);
void enc28j60_set_rxrdpt(uint16_t ptr);
uint8_t enc28j60_packet_pending(void);
void uart_hexdump(const uint8_t* data, uint16_t len);
void enc28j60_set_bank(uint8_t addr);
void enc28j60_write_phy(uint8_t addr, uint16_t data);
uint16_t enc28j60_packet_receive(uint8_t* pkt, uint16_t max_len);
void enc28j60_packet_send(const uint8_t* pkt, uint16_t len);
void write_reg_2b(uint8_t addr, uint16_t data);
uint16_t enc28j60_read_phy(uint8_t addr);

#endif
// written by an0n9m (Davit Jalagonia)
