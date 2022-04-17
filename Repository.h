#ifndef MY_RECORD
#define MY_RECORD

#include <inttypes.h>
#include <stdbool.h>

typedef struct 
{
    uint32_t addr;
    uint8_t mask;
    uint32_t distance;
    uint8_t silentToursN;
    uint32_t nextAddr;
} Record;

typedef struct {
    Record **records;
    unsigned n;
} Repository;


void Repository_Init();
void Repository_Free();

Repository *Repository_GetAlive();
Repository *Repository_GetDirectly();

/* ---> Repository nonstatic <--- */

unsigned Repository_getSize(Repository *repo);
Record *Repository_getEntry(Repository *repo, uint32_t addr);
Record *Repository_getEntryByNext(Repository *repo, uint32_t addr);
bool Repository_containsEntry(Repository *repo, uint32_t addr);
Record *Repository_addEntry(Repository *repo, uint32_t addr, uint8_t mask, uint32_t next, uint32_t distance);
void Repository_removeEntry(Repository *repo, Record *record);

#endif