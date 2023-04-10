#include "../headers/darray.h"
#include <stdlib.h>

void darray_create(unsigned int capacity, darray *instance)
{
    instance = malloc(sizeof(unsigned int) * capacity + sizeof(unsigned int) + sizeof(void *));
    instance->capacity = capacity;
    instance->count = 0;
    instance->items = malloc(sizeof(void *));
}