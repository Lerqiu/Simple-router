#include <inttypes.h>
#include "IP_helper.h"
#include "Repository.h"
#include <endian.h>

uint32_t IP_Broadcast(uint32_t addr, uint8_t mask)
{
    return addr | ((1 << (32 - mask)) - 1);
}

uint32_t IP_Network(uint32_t addr, uint8_t mask)
{
    return addr & ~((1 << (32 - mask)) - 1);
}


uint8_t *Record_to_udpMessage(Record *record)
{
    static uint8_t message[9];
    uint32_t addr = IP_Network(record->addr, record->mask);
    uint8_t mask = record->mask;
    uint32_t distance = record->distance;
    *((uint32_t *)&message[0]) = htobe32(addr);
    *((uint8_t *)&message[4]) = mask;
    *((uint32_t *)&message[5]) = htobe32(distance);
    return message;
}

uint32_t UdpMessage_to_addr(uint8_t message[])
{
    return be32toh(*((uint32_t *)&message[0]));
}

uint32_t UdpMessage_to_distance(uint8_t message[])
{
    return be32toh(*((uint32_t *)&message[5]));
}

uint8_t UdpMessage_to_mask(uint8_t message[])
{
    return message[4];
}