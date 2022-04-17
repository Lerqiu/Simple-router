#include "Repository.h"
#include "Input.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <assert.h>

static Record **repoData = NULL;
static unsigned repoSize = 0;

void Repository_init()
{
    assert(repoData == NULL && repoSize == 0);

    unsigned inputSize = Input_getN();

    repoData = malloc(sizeof(Record *) * inputSize);
    repoSize = inputSize;

    for (int i = 0; i < inputSize; i++)
    {
        repoData[i] = Input_getRecord();
    }

    assert(repoData != NULL && repoSize > 0);
}

void Repository_free()
{
    assert(repoData != NULL && repoSize > 0);

    for (int i = 0; i < repoSize; i++)
    {
        free(repoData[i]);
    }

    repoData = NULL;
    repoSize = 0;

    assert(repoData == NULL && repoSize == 0);
}

Record **Repository_getAll()
{
    return repoData;
}

Record *Repository_get(uint32_t addr)
{
    for (int i = 0; i < repoSize; i++)
    {
        if (repoData[i]->addr == addr)
            return repoData[i];
    }
    assert(!Repository_contains(addr));
    fprintf(stderr, "Repository don't contains %u\n", addr);
    exit(EXIT_FAILURE);
}

unsigned Repository_getSize()
{
    return repoSize;
}

bool Repository_contains(uint32_t addr)
{
    for (int i = 0; i < repoSize; i++)
    {
        if (repoData[i]->addr == addr)
            return true;
    }
    return false;
}

Record *Repository_add(uint32_t addr, uint8_t mask, uint32_t next, uint32_t distance)
{
    Record *record = malloc(sizeof(Record));
    assert(record != NULL);
    record->silentToursN = 0;
    record->isActive = true;
    record->isDirectly = false;
    record->nextAddr = next;
    record->addr = addr;
    record->mask = mask;
    record->distance = distance;

    repoData = realloc(repoData, sizeof(Record *) * ++repoSize);
    repoData[repoSize - 1] = record;

    return record;
}

void Repository_remove(Record *record)
{
    assert(record && repoSize > 0);
    Record **newData = malloc(sizeof(Record *) * (repoSize - 1));
    for (unsigned old = 0, new = 0; old < repoSize; old++)
    {
        Record *br = repoData[old];
        if (br == record)
            continue;
        newData[new] = repoData[old];
        new ++;
    }
    free(repoData);
    repoData = newData;
    repoSize--;
}

Record *Repository_getNext(uint32_t addr)
{
    for (int i = 0; i < repoSize; i++)
    {
        Record *record = repoData[i];
        if (record->nextAddr == addr && record->isDirectly)
            return record;
    }
    return NULL;
}