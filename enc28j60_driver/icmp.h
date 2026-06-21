#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>

// Timing
#define WINDOW_MS 500
#define MS_PER_LOOP 50

// State
static uint8_t ping_count = 0;
static uint16_t window_timer = 0;
static uint8_t window_active = 0;
static uint8_t gpio_state = 0;

// Offsets
#define IP_DST      30
#define IP_SRC      26
#define IP_PROTO    23
#define ICMP_TYPE   34
#define ICMP_CSUM   36
#define ETH_DST     0
#define ETH_SRC     6

void gpio_init(void);

void icmp_tick(void);

void handle_icmp(uint8_t* pkt, uint16_t len, const uint8_t* my_mac, const uint8_t* my_ip);

#endif
// written by an0n9m (Davit Jalagonia)
