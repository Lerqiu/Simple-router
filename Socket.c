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

int Socket_create()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
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
    timeout.tv_sec = 0;
    timeout.tv_usec = time;

    while (timeout.tv_sec > 0 || timeout.tv_usec > 0)
    {
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        int selectResult = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        assert(selectResult >= 0);

        if (FD_ISSET(sockfd, &readfds))
            printf("Some package come");
    }
    Output_all();
}

void Socket_send(int sockfd)
{
    Record **records = Repository_getAll();
    int n = Repository_getSize();

    for (int i = 0; i < n; i++)
    {
        Record *r = records[i];
        if (!r->isDirectly)
            continue;

        struct sockaddr_in address;
        bzero(&address, sizeof(address));

        address.sin_family = AF_INET;
        address.sin_port = htons(54321);
        address.sin_addr.s_addr = htons(htons(r->addr) | ~((1 << (32 - r->mask )) - 1));

        char *message = "Hello from hell!";
        ssize_t message_len = strlen(message);
        if (sendto(sockfd, message, message_len, 0, (struct sockaddr *)&address, sizeof(address)) != message_len)
        {
            fprintf(stderr, "sendto error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}