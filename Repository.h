#ifndef MY_RECORD
#define MY_RECORD

#include <inttypes.h>
#include <stdbool.h>

typedef struct 
{
    uint32_t addr;
    uint8_t mask;
    uint32_t distance;
    bool isDirectly;
    bool isActive;
    uint8_t silentToursN;
    uint32_t nextAddr;
} Record;

void Repository_init();
void Repository_free();

Record ** Repository_getAll();
unsigned Repository_getSize();

#endif