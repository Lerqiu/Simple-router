#ifndef MY_IP_HELPER
#define MY_IP_HELPER

#include <inttypes.h>

uint32_t IP_Broadcast(uint32_t addr, uint8_t mask);
uint8_t *Record_to_udpMessage(Record * record);

#define UDP_MESSAGE_SIZE 9


uint32_t UdpMessage_to_addr(uint8_t message[]);
uint32_t UdpMessage_to_distance(uint8_t message[]);
uint8_t UdpMessage_to_mask(uint8_t message[]);

#endif