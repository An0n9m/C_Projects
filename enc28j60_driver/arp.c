// arp.c
#include "arp.h"
#include "enc28j60.h"
#include "uart.h"
#include <string.h>

uint8_t parse_ethernet(uint8_t* pkt, uint16_t len, eth_frame_t* frame)
{
    if (len < 14)
        return 0;

    // Copy MAC addresses
    memcpy(frame->dst_mac, &pkt[ETH_DST_MAC], 6);
    memcpy(frame->src_mac, &pkt[ETH_SRC_MAC], 6);

    // Get Ethertype 
    frame->eth_type = (pkt[ETH_TYPE] << 8) | pkt[ETH_TYPE + 1];

    // Payload
    frame->payload = &pkt[ETH_PAYLOAD];
    frame->payload_len = len - ETH_PAYLOAD;

    return 1;
}

uint8_t parse_arp(uint8_t* arp_data, arp_packet_t* arp)
{
    // Hardware type 
    arp->hw_type = (arp_data[ARP_HTYPE] << 8) | arp_data[ARP_HTYPE + 1];
    
    // Protocol type 
    arp->proto_type = (arp_data[ARP_PTYPE] << 8) | arp_data[ARP_PTYPE + 1];
    
    // Lengths
    arp->hw_len = arp_data[ARP_HLEN];
    arp->proto_len = arp_data[ARP_PLEN];
    
    // Operation
    arp->operation = (arp_data[ARP_OPER] << 8) | arp_data[ARP_OPER + 1];
    
    // Addresses
    memcpy(arp->sender_mac, &arp_data[ARP_SHA], 6);
    memcpy(arp->sender_ip, &arp_data[ARP_SPA], 4);
    memcpy(arp->target_mac, &arp_data[ARP_THA], 6);
    memcpy(arp->target_ip, &arp_data[ARP_TPA], 4);

    return 1;
}

void send_arp_reply(const uint8_t* my_mac, const uint8_t* my_ip,
                    const uint8_t* target_mac, const uint8_t* target_ip)
{
    uint8_t reply[42]; // 42 bytes
    
    // Ethernet header
    memcpy(&reply[ETH_DST_MAC], target_mac, 6);  // Destination
    memcpy(&reply[ETH_SRC_MAC], my_mac, 6);      // Source
    reply[ETH_TYPE] = 0x08;                      // Type: ARP
    reply[ETH_TYPE + 1] = 0x06;
    
    // ARP header
    uint8_t* arp = &reply[ETH_PAYLOAD];
    
    arp[ARP_HTYPE] = 0x00;      // Hardware type: Ethernet
    arp[ARP_HTYPE + 1] = 0x01;
    
    arp[ARP_PTYPE] = 0x08;      // Protocol type: IPv4
    arp[ARP_PTYPE + 1] = 0x00;
    
    arp[ARP_HLEN] = 6;          // Hardware length
    arp[ARP_PLEN] = 4;          // Protocol length
    
    arp[ARP_OPER] = 0x00;       // Operation: Reply
    arp[ARP_OPER + 1] = 0x02;
    
    // Sender 
    memcpy(&arp[ARP_SHA], my_mac, 6);
    memcpy(&arp[ARP_SPA], my_ip, 4);
    
    // Target 
    memcpy(&arp[ARP_THA], target_mac, 6);
    memcpy(&arp[ARP_TPA], target_ip, 4);
    
    // Send packet
    enc28j60_packet_send(reply, 42);
    
    uart_puts("ARP reply sent to ");
    print_ip(target_ip);
    uart_puts("\r\n");
}

void print_mac(const uint8_t* mac)
{
    for (uint8_t i = 0; i < 6; i++) {
        uart_puthex(mac[i]);
        if (i < 5) uart_putc(':');
    }
}

void print_ip(const uint8_t* ip)
{
    for (uint8_t i = 0; i < 4; i++) {
        uart_putdec(ip[i]);
        if (i < 3) uart_putc('.');
    }
}

uint8_t ip_compare(const uint8_t* ip1, const uint8_t* ip2)
{
    return (ip1[0] == ip2[0] && ip1[1] == ip2[1] && 
            ip1[2] == ip2[2] && ip1[3] == ip2[3]);
}
// written by an0n9m (Davit Jalagonia)
