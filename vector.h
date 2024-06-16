#ifndef VECTOR
#define VECTOR

typedef struct vector_t
{
    void **arr;
    unsigned int size;
    unsigned int capacity;
} vector_t;

vector_t *init_vector();
void push_back(vector_t *, void *);
void clear(vector_t *);

void *getAt(vector_t *, int); 
void setAt(vector_t *v, int index, void *val);
int getSize(vector_t *);

#endif