typedef struct darray 
{
    void **items;
    unsigned int capacity;
    unsigned int count;
}darray;

void darray_create(unsigned int capacity, darray *instance);
int darray_add(void *item, darray *instance);
void darray_destroy(darray *instance);