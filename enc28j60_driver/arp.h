#ifndef ARP_H
#define ARP_H

#include <stdint.h>

// Ethernet frame offsets
#define ETH_DST_MAC     0   // 6 bytes
#define ETH_SRC_MAC     6   // 6 bytes
#define ETH_TYPE        12  // 2 bytes
#define ETH_PAYLOAD     14

// Ethernet types
#define ETH_TYPE_ARP    0x0806
#define ETH_TYPE_IP     0x0800

// ARP packet offsets
#define ARP_HTYPE       0   // Hardware type (2 bytes)
#define ARP_PTYPE       2   // Protocol type (2 bytes)
#define ARP_HLEN        4   // Hardware length (1 byte)
#define ARP_PLEN        5   // Protocol length (1 byte)
#define ARP_OPER        6   // Operation (2 bytes)
#define ARP_SHA         8   // Sender hardware addr (6 bytes)
#define ARP_SPA         14  // Sender protocol addr (4 bytes)
#define ARP_THA         18  // Target hardware addr (6 bytes)
#define ARP_TPA         22  // Target protocol addr (4 bytes)

// ARP operations
#define ARP_OP_REQUEST  1
#define ARP_OP_REPLY    2

typedef struct {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t eth_type;
    uint8_t* payload;
    uint16_t payload_len;
} eth_frame_t;

typedef struct {
    uint16_t hw_type;
    uint16_t proto_type;
    uint8_t hw_len;
    uint8_t proto_len;
    uint16_t operation;
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
} arp_packet_t;

// Parse Ethernet frame
uint8_t parse_ethernet(uint8_t* pkt, uint16_t len, eth_frame_t* frame);

// Parse ARP packet
uint8_t parse_arp(uint8_t* arp_data, arp_packet_t* arp);

// Build and send ARP reply
void send_arp_reply(const uint8_t* my_mac, const uint8_t* my_ip, 
                    const uint8_t* target_mac, const uint8_t* target_ip);

// Helper func
void print_mac(const uint8_t* mac);
void print_ip(const uint8_t* ip);
uint8_t ip_compare(const uint8_t* ip1, const uint8_t* ip2);

#endif // ARP_H
// written by an0n9m (Davit Jalagonia)
