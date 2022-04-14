#include "Repository.h"
#include "Input.h"
#include <stdlib.h>
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

Record ** Repository_getAll(){
    return repoData;
}

unsigned Repository_getSize(){
    return repoSize;
}