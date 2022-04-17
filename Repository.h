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
Record *Repository_get(uint32_t addr);
unsigned Repository_getSize();

bool Repository_contains(uint32_t addr);

Record *Repository_add(uint32_t addr, uint8_t mask, uint32_t next,uint32_t distance);
Record *Repository_getNext(uint32_t addr);

void Repository_remove(Record *record);

#endif