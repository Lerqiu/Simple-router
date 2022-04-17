#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "Input.h"
#include "Repository.h"
#include "IP_helper.h"

Record *Input_getRecord()
{
    static unsigned counter = 0;

    Record *record = malloc(sizeof(Record));
    assert(record != NULL);
    record->silentToursN = 0;

    uint8_t a1,a2,a3,a4;
    int rr;
    if (6 != (rr = fscanf(stdin, "%hhu.%hhu.%hhu.%hhu/%hhu distance %u\n",&a1,&a2,&a3,&a4, &record->mask,&record->distance)))
    {
        fprintf(stderr, "Wrong record format | N: %u | %d\n", ++counter,rr);
        exit(EXIT_FAILURE);
    }
    record->nextAddr = (a1 << (3*8)) | (a2 << (2*8)) | (a3 << (1*8)) | a4;
    record->addr = IP_Network(record->nextAddr,record->mask);
    return record;
}

unsigned Input_getN()
{
    static unsigned n;
    static bool cached = false;

    if (cached)
        return n;

    if (1 != fscanf(stdin, "%u\n", &n))
    {
        fprintf(stderr, "Wrong config format\n");
        exit(EXIT_FAILURE);
    }
    cached = true;
    return n;
}
