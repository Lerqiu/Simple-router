#include <inttypes.h>
#include "IP_helper.h"
#include "Repository.h"
#include <endian.h>

uint32_t IP_Broadcast(uint32_t addr, uint8_t mask)
{
    return addr | ((1 << (32 - mask)) - 1);
}

uint8_t *Record_to_udpMessage(Record *record)
{
    uint8_t message[9];
    uint32_t addr = IP_Broadcast(record->addr, record->mask);
    uint8_t mask = record->mask;
    uint32_t distance = record->distance;
    // message[0] = (addr >> (3 * 8)) && 0xFF;
    // message[1] = (addr >> (2 * 8)) && 0xFF;
    // message[2] = (addr >> (1 * 8)) && 0xFF;
    // message[3] = (addr >> (0 * 8)) && 0xFF;
    // message[4] = record->mask;
    // message[5] = (distance >> (3 * 8)) && 0xFF;
    // message[6] = (distance >> (2 * 8)) && 0xFF;
    // message[7] = (distance >> (1 * 8)) && 0xFF;
    // message[8] = (distance >> (0 * 8)) && 0xFF;
    *((uint32_t *)&message[0]) = htobe32(addr);
    *((uint8_t *)&message[4]) = mask;
    *((uint32_t *)&message[5]) = htobe32(distance);
    return message;
}

uint32_t UdpMessage_to_addr(uint8_t message[])
{
    // return (uint32_t)message[3] |
    //        ((uint32_t)message[2] << (1 * 8)) |
    //        ((uint32_t)message[1] << (2 * 8)) |
    //        ((uint32_t)message[2] << (3 * 8));
    return be32toh(*((uint32_t *)&message[0]));
}

uint32_t UdpMessage_to_distance(uint8_t message[])
{
    // return (uint32_t)message[8] |
    //        ((uint32_t)message[7] << (1 * 8)) |
    //        ((uint32_t)message[6] << (2 * 8)) |
    //        ((uint32_t)message[5] << (3 * 8));

    return be32toh(*((uint32_t *)&message[5]));
}

uint8_t UdpMessage_to_mask(uint8_t message[])
{
    return message[4];
}