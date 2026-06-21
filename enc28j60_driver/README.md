\# ENC28J60 Ethernet Driver for AVR



A complete ENC28J60 Ethernet controller driver written entirely in C for AVR microcontrollers.



The project was developed and tested on an ATmega328P (Arduino Uno), but can be easily adapted to other AVR microcontrollers with minor modifications.



Besides the low-level ENC28J60 driver, the project implements a minimal Ethernet stack capable of handling ARP and ICMP protocols and controlling GPIO outputs through network traffic.



\---



\## Features



\* Full ENC28J60 driver implementation

\* SPI communication layer

\* UART debugging interface

\* Ethernet frame parsing

\* ARP packet processing

\* ARP reply generation

\* ICMP Echo Request (Ping) processing

\* ICMP Echo Reply generation

\* RX/TX buffer management

\* PHY register access

\* MAC address configuration

\* Network-controlled GPIO outputs

\* AVR-GCC compatible



\---



\## Hardware Connections



\### ATmega328P ↔ ENC28J60



| ATmega328P | Arduino Pin | ENC28J60 |

| ---------- | ----------- | -------- |

| PB2        | D10         | CS       |

| PB3        | D11         | MOSI     |

| PB4        | D12         | MISO     |

| PB5        | D13         | SCK      |



Communication between the microcontroller and the ENC28J60 is performed through the SPI protocol.



UART is used for debugging and monitoring the device state during runtime.



\---



\## Network Configuration



Network parameters can be modified directly inside `main.c`.



```c

uint8\_t my\_mac\[6] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

uint8\_t my\_ip\[4] = {192, 168, 0, 57};

```



After startup, initialization messages are printed through UART and the device begins listening for Ethernet traffic.



\---



\## Supported Protocols



\### ARP (Address Resolution Protocol)



The driver listens for incoming ARP requests.



When an ARP request targets the configured IP address:



1\. The Ethernet frame is parsed.

2\. The target IP address is verified.

3\. An ARP reply is generated.

4\. The requester receives the configured MAC address.



This allows other hosts on the network to discover the device.



\---



\### ICMP (Internet Control Message Protocol)



The project supports ICMP Echo Requests (Ping).



When a valid Echo Request is received:



1\. The destination IP address is verified.

2\. The Ethernet and IP headers are modified.

3\. The ICMP type is changed from Request to Reply.

4\. The checksum is updated.

5\. The packet is transmitted back to the sender.



The device therefore behaves as a pingable network node.



\---



\## GPIO Control Through Network Traffic



A custom feature has been implemented on top of ICMP.



Incoming ping packets are counted within a fixed time window.



Depending on the number of received ICMP packets, a GPIO output is toggled.



\### Mapping



| Ping Count | Arduino Pin |

| ---------- | ----------- |

| 1          | D2          |

| 2          | D3          |

| 3          | D4          |

| 4          | D5          |

| 5          | D6          |

| 6          | D7          |

| 7          | D8          |

| 8          | D9          |



Sending the same number of pings again toggles the corresponding output back OFF.



This provides a simple network-controlled digital I/O interface without requiring TCP, UDP, or application-layer protocols.



\---



\## Driver Architecture



\### SPI Layer



Provides low-level communication with the ENC28J60.



Functions:



\* `spi\_init()`

\* `spi\_transfer()`

\* `spi\_read\_last()`



\---



\### ENC28J60 Core Driver



Main driver functionality:



\* Bank switching

\* Register access

\* PHY access

\* Buffer management

\* Packet transmission

\* Packet reception



Important functions:



```c

enc28j60\_set\_bank()

enc28j60\_read\_op()

enc28j60\_write\_op()

enc28j60\_read\_reg()

enc28j60\_write\_reg()

enc28j60\_read\_buffer()

enc28j60\_write\_buffer()

enc28j60\_packet\_receive()

enc28j60\_packet\_send()

enc28j60\_full\_init()

```



\---



\### Ethernet / ARP Layer



Responsible for:



\* Ethernet frame parsing

\* ARP packet parsing

\* ARP reply generation



Functions:



```c

parse\_ethernet()

parse\_arp()

send\_arp\_reply()

```



\---



\### ICMP Layer



Responsible for:



\* Echo Request detection

\* Echo Reply generation

\* Ping counting

\* GPIO control logic



Functions:



```c

handle\_icmp()

icmp\_tick()

gpio\_set()

gpio\_toggle()

```



\---



\## ENC28J60 Initialization



The full initialization routine performs:



\* Software reset

\* RX buffer configuration

\* TX buffer configuration

\* Packet filtering configuration

\* MAC configuration

\* PHY configuration

\* Interrupt enabling

\* Packet reception enabling



After initialization the controller is ready to receive and transmit Ethernet frames.



\---



\## Building



Compile using AVR-GCC:



```bash

./compile.sh

```



The script:



1\. Compiles all source files

2\. Generates a HEX file

3\. Uploads firmware using avrdude

4\. Opens a UART terminal at 115200 baud



\---



\## Current Protocol Support



Implemented:



\* ARP

\* ICMP



Future protocols can be added on top of the existing Ethernet parsing layer:



\* UDP

\* TCP

\* DHCP

\* DNS

\* HTTP



\---



\## Author



Davit Jalagonia (An0n9m)



Written entirely in C for AVR microcontrollers.



