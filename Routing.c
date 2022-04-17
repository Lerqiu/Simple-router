#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "Repository.h"
#include "IP_helper.h"
#include "Main.h"
#include <limits.h>

void Routing_receive(int sockfd)
{
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    u_int8_t buffer[IP_MAXPACKET + 1];

    ssize_t datagram_len = recvfrom(sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr *)&sender, &sender_len);
    if (datagram_len < 0)
    {
        fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (datagram_len == 9 && Repository_contains(be32toh(sender.sin_addr.s_addr)))
    {
        uint32_t addr = UdpMessage_to_addr(buffer);
        uint8_t mask = UdpMessage_to_mask(buffer);
        uint32_t distance = UdpMessage_to_distance(buffer);
        uint32_t from = be32toh(sender.sin_addr.s_addr);
        printf("Massage: From<->%u Network<->%u Mask<->%u", from, from, mask);

        Record *recordNext = Repository_getNext(from);
        if (recordNext)
        {
            if (Repository_contains(addr))
            {
                Record *record = Repository_get(addr);
                recordNext->silentToursN = 0;
                record->silentToursN = 0;
                record->isActive = true;
                uint32_t distanceProposition = record->distance + recordNext->distance;
                uint32_t distance = record->distance;
                if (distance > distanceProposition)
                {
                    record->distance = distanceProposition;
                    record->nextAddr = from;
                }
            }
            else
                Repository_add(addr, mask, from, distance);
        }
    }
}

void Routing_removeOld()
{
    Record **records = Repository_getAll();
    unsigned n = Repository_getSize();

    for (int i = 0; i < n; i++)
    {
        Record *record = records[i];
        if (!record->isActive)
            continue;

        if (record->silentToursN >= MAX_ALIVE || record->distance >= MAX_DISTANCE)
            record->distance = UINT_MAX;

        if (record->distance >= MAX_DISTANCE)
            record->silentToursN = record->silentToursN > MAX_ALIVE ? record->silentToursN : MAX_ALIVE;

        if (record->silentToursN > REMOVE_THRESHOLD)
        {
            if (record->isDirectly)
            {
                record->isActive = false;
                continue;
            }
            i--;
            Repository_remove(records);
            records = Repository_getAll();
            n = Repository_getSize();
        }
    }
}

void Routing_age()
{
    Record **records = Repository_getAll();
    unsigned n = Repository_getSize();

    for (int i = 0; i < n; i++)
    {
        records[i]->silentToursN++;
    }
}

void Routing_notifyAvailable(Record *record)
{
    record->silentToursN = 0;
    record->isActive = true;
}