#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

vector_t *init_vector()
{
    vector_t *v = (vector_t *)malloc(sizeof(vector_t));
    v->arr = NULL;
    v->size = 0;
    v->capacity = 0;

    return v;
}

void resize(vector_t *v, int newCapacity)
{
    void **tmp = (void **)realloc(v->arr, newCapacity * sizeof(void *));
    if (tmp == NULL)
    {
        exit(1);
    }

    v->arr = tmp;
    v->capacity = newCapacity;
}

void push_back(vector_t *v, void *val)
{
    if (v->size == v->capacity)
    {
        if (v->capacity == 0)
        {
            v->capacity = 2;
        }
        else
        {
            v->capacity *= 2;
        }

        resize(v, v->capacity);
    }

    v->arr[v->size] = val;
    v->size++;
}

void clear(vector_t *v)
{
    free(v->arr);
    v->arr = NULL;
    v->size = 0;
    v->capacity = 0;
}


void *getAt(vector_t *v, int index)
{
    if (index >= v->size)
    {
        return NULL;
        // exit(1);
    }

    return v->arr[index];
}

void setAt(vector_t *v, int index, void *val) {
    if (index >= v->size) {
        return;
    }
    v->arr[index] = val;
}

int getSize(vector_t *v)
{
    return v->size;
}
