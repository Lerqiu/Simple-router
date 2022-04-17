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
#include "Output.h"

static bool _isAddrOfNeighbour(uint32_t addr)
{
    Repository *directly = Repository_GetDirectly();
    return Repository_getEntryByNext(directly, addr);
}

static void _merge(Repository *repo, Record *record)
{
    if (Repository_containsEntry(repo, record->addr))
    {
        Record *oldEntry = Repository_getEntry(repo, record->addr);
        if (oldEntry->distance > record->distance)
        {
            oldEntry->distance = record->distance;
            oldEntry->nextAddr = record->nextAddr;
        }

        Repository *directly = Repository_GetDirectly();
        if (record->nextAddr == oldEntry->nextAddr &&
            !Repository_containsEntry(directly, record->addr))
        {
            oldEntry->distance = record->distance;
        }
        oldEntry->silentToursN = 0;
        printf("Modyfikacja wpisu: ");
        Output_one(oldEntry);
    }
    else
    {
        Repository_addEntry(repo, record->addr, record->mask, record->nextAddr, record->distance);
        printf("Dodawanie wpisu: ");
        Output_one(record);
    }
}

void _updateRoutingTable(Record *received)
{
    Repository *RAlive = Repository_GetAlive();
    Repository *RDirectly = Repository_GetDirectly();

    Record *prevNext = Repository_getEntryByNext(RDirectly, received->nextAddr);
    _merge(RAlive, prevNext);
    received->distance = received->distance == UINT_MAX ? UINT_MAX : prevNext->distance + received->distance;
    _merge(RAlive, received);
}

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

    if (datagram_len == 9 && _isAddrOfNeighbour(be32toh(sender.sin_addr.s_addr)))
    {
        Record record;
        record.addr = UdpMessage_to_addr(buffer);
        record.mask = UdpMessage_to_mask(buffer);
        record.distance = UdpMessage_to_distance(buffer);
        record.nextAddr = be32toh(sender.sin_addr.s_addr);
        record.silentToursN = 0;

        // printf("Massage: From<->%u Network<->%u Mask<->%u", from, from, mask);
        _updateRoutingTable(&record);
    }
}

void Routing_removeOld()
{
    Repository *RAlive = Repository_GetAlive();

    for (unsigned i = 0; i < RAlive->n; i++)
    {
        Record *record = RAlive->records[i];

        if (record->silentToursN >= MAX_ALIVE || record->distance >= MAX_DISTANCE)
            record->distance = UINT_MAX;

        if (record->distance >= MAX_DISTANCE)
            record->silentToursN = record->silentToursN > MAX_ALIVE ? record->silentToursN : MAX_ALIVE;

        if (record->silentToursN > REMOVE_THRESHOLD)
        {
            i--;
            if (Repository_getEntryByNext(Repository_GetDirectly(), record->nextAddr))
            {
                for (unsigned y = 0; y < RAlive->n; y++)
                    if (RAlive->records[y]->nextAddr == record->nextAddr)
                        RAlive->records[y]->distance = MAX_DISTANCE;
            }
            printf("Usuwanie wpisu: ");
            Output_one(record);
            Repository_removeEntry(RAlive, record);
        }
    }
}

void Routing_age()
{
    Repository *RAlive = Repository_GetAlive();
    for (unsigned i = 0; i < RAlive->n; i++)
        RAlive->records[i]->silentToursN++;
}