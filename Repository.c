#include "Repository.h"
#include "Input.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <assert.h>

/* ---> Variables <--- */

static Repository RAlive = {.n = 0, .records = NULL};
static Repository RDirectly = {.n = 0, .records = NULL};

/* ---> Repository static <--- */

static Record *_copyRecord(Record *record)
{
    Record *new = malloc(sizeof(Record));
    new->addr = record->addr;
    new->distance = record->distance;
    new->mask = record->mask;
    new->nextAddr = record->nextAddr;
    new->silentToursN = record->silentToursN;
    return new;
}

void Repository_Init()
{
    RDirectly.n = Input_getN();
    RAlive.n = RDirectly.n;

    RDirectly.records = malloc(sizeof(Record *) * RDirectly.n);
    RDirectly.records = malloc(sizeof(Record *) * RDirectly.n);

    for (int i = 0; i < RDirectly.n; i++)
    {
        RDirectly.records[i] = Input_getRecord();
        RAlive.records[i] = _copyRecord(RDirectly.records[i]);
    }
}
static void _free(Repository *repo)
{
    for (int i = 0; i < repo->n; i++)
    {
        free(repo->records[i]);
    }
    repo->n = 0;
    repo->records = NULL;
}

void Repository_Free()
{
    _free(&RAlive);
    _free(&RDirectly);
}

Repository *Repository_GetAlive()
{
    return &RAlive;
}

Repository *Repository_GetDirectly()
{
    return &RDirectly;
}

/* ---> Repository nonstatic <--- */

unsigned Repository_getSize(Repository *repo)
{
    return repo->n;
}

Record *Repository_getEntry(Repository *repo, uint32_t addr)
{
    for (int i = 0; i < repo->n; i++)
    {
        if (repo->records[i]->addr == addr)
            return repo->records[i];
    }

    fprintf(stderr, "Repository don't contains %u\n", addr);
    exit(EXIT_FAILURE);
}

Record *Repository_getEntryByNext(Repository *repo, uint32_t addr)
{
    for (int i = 0; i < repo->n; i++)
    {
        if (repo->records[i]->nextAddr == addr)
            return repo->records[i];
    }
    return NULL;
}

bool Repository_containsEntry(Repository *repo, uint32_t addr)
{
    for (int i = 0; i < repo->n; i++)
    {
        if (repo->records[i]->addr == addr)
            return true;
    }
    return false;
}

Record *Repository_addEntry(Repository *repo, uint32_t addr, uint8_t mask, uint32_t next, uint32_t distance)
{
    Record *record = malloc(sizeof(Record));
    assert(record != NULL);
    record->silentToursN = 0;
    record->nextAddr = next;
    record->addr = addr;
    record->mask = mask;
    record->distance = distance;

    repo->records = realloc(repo->records, sizeof(Record *) * ++repo->n);
    repo->records[repo->n - 1] = record;

    return record;
}

void Repository_removeEntry(Repository *repo, Record *record)
{
    Record **newData = malloc(sizeof(Record *) * (repo->n - 1));
    for (unsigned old = 0, new = 0; old < repo->n; old++)
    {
        Record *repoEntry = repo->records[old];
        if (repoEntry == record)
        {
            free(record);
            continue;
        }
        newData[new] = repoEntry;
        new ++;
    }
    free(repo->records);
    repo->records = newData;
    repo->n--;
}