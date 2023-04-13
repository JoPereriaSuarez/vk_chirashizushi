#include "darray.h"
#include <stdlib.h>

void darray_create(unsigned int capacity, darray *instance)
{
    instance = malloc(sizeof(unsigned int) * capacity + sizeof(unsigned int) + sizeof(void *));
    instance->capacity = capacity;
    instance->count = 0;
    instance->items = malloc(sizeof(void *) * capacity);
}

int darray_add(void *item, darray *instance)
{
    if(instance->count >= instance->capacity-1)
    {
        instance->items[instance->count] = malloc(sizeof(void *));
        instance->capacity ++;
    }

    instance->items[instance->count] = item;
    instance->count++;
}

void darray_destroy(darray *instance)
{
    for(size_t i = 0; i < instance->capacity; i++)
    {
        free(instance->items[i]);
        instance->items[i] = NULL;
    }

    free(instance->items);
    instance->items = NULL;
    free(instance);
    instance = NULL;
}