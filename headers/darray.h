typedef struct darray 
{
    void **items;
    unsigned int capacity;
    unsigned int count;
}darray;

void darray_create(unsigned int capacity, darray *instance);
int darray_add(void **items);
int darray_destroy(darray *instance);