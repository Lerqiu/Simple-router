#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "Repository.h"
#include <endian.h>
#include <string.h>
#include <errno.h>

#define msg(...)                                                               \
    {                                                                          \
        if (!printf(__VA_ARGS__))                                              \
        {                                                                      \
            fprintf(stderr, "Output_one, printf error %s\n", strerror(errno)); \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    }

static void _printAddr(uint32_t addr)
{
    msg("%u.%u.%u.%u",
        (addr >> (3 * 8)) & 0xFF,
        (addr >> (2 * 8)) & 0xFF,
        (addr >> (1 * 8)) & 0xFF,
        (addr >> (0 * 8)) & 0xFF)
}

void Output_one(Record *record)
{
    // static unsigned counter = 0;
    // printf("W:%u\n",counter++);
    
    _printAddr(record->addr);
    printf("/%u ", record->mask);

    msg("distance %u ", record->distance);

    if (Repository_containsEntry(Repository_GetDirectly(), record->addr))
        msg("connected directly\n") else
        {
            _printAddr(record->nextAddr);
            msg("\n");
        }
}

void Output_all()
{
    Repository *repo = Repository_GetAlive();

    for (unsigned i = 0; i < repo->n; i++)
        Output_one(repo->records[i]);
}