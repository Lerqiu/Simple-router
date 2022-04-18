#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "Repository.h"
#include <assert.h>
#include "Output.h"
#include "Routing.h"
#include "IP_helper.h"
#include "Main.h"

int Socket_create()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int broadcastEnable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)))
    {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(54321);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void Socket_listening(int sockfd, unsigned long time)
{
    struct timeval timeout;
    timeout.tv_sec = time;
    timeout.tv_usec = 0;

    while (timeout.tv_sec > 0 || timeout.tv_usec > 0)
    {
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        int selectResult = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        assert(selectResult >= 0);

        if (FD_ISSET(sockfd, &readfds))
            Routing_receive(sockfd);
    }
    Output_all();
}

static void _send(Record *record, int sockfd)
{
    Repository *directly = Repository_GetDirectly();

    for (unsigned i = 0; i < directly->n; i++)
    {
        Record *neighbor = directly->records[i];

        struct sockaddr_in address;
        bzero(&address, sizeof(address));

        address.sin_family = AF_INET;
        address.sin_port = htons(54321);
        address.sin_addr.s_addr = htobe32(IP_Broadcast(neighbor->nextAddr, neighbor->mask));

        if (sendto(sockfd, Record_to_udpMessage(record), UDP_MESSAGE_SIZE, 0, (struct sockaddr *)&address, sizeof(address)) != UDP_MESSAGE_SIZE)
        {
            neighbor->distance = MAX_DISTANCE;
            if (record->nextAddr == neighbor->nextAddr)
                record->distance = MAX_DISTANCE;
        }
    }
}

static void _send_aliveNotify(Record *neighbor, int sockfd)
{
    struct sockaddr_in address;
    bzero(&address, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(54321);
    address.sin_addr.s_addr = htobe32(IP_Broadcast(neighbor->nextAddr, neighbor->mask));

    if (sendto(sockfd, Record_to_udpMessage(neighbor), UDP_MESSAGE_SIZE, 0, (struct sockaddr *)&address, sizeof(address)) == UDP_MESSAGE_SIZE)
    {
        neighbor->silentToursN = 0;
    }else{
        printf("Error: %s\n",strerror(errno));
    }
}

void Socket_send(int sockfd)
{
    Repository *repo = Repository_GetAlive();

    for (unsigned i = 0; i < repo->n; i++)
        _send(repo->records[i], sockfd);

    repo = Repository_GetDirectly();
    for (unsigned i = 0; i < repo->n; i++)
        _send_aliveNotify(repo->records[i], sockfd);
}