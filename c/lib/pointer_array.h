#ifndef POINTER_ARRAY_H
#define POINTER_ARRAY_H

#include "nicetypes.h"
#include "misc.h"

#include <stdlib.h>
struct Pool;
void *get_memory(struct Pool *pool, s64 nbytes);
void *get_more_memory(struct Pool *pool, void *old_memory, s64 nbytes, s64 old_nbytes);

// Set pool to NULL to use malloc as allocator
typedef struct
{
    u8 **data;
    s64 count;
    s64 capacity;
    
    struct Pool *pool;
} PointerArray;

PointerArray make_pointer_array(s64 capacity, struct Pool *pool)
{
    PointerArray array;
    array.count = 0;
    array.capacity = 0;
    array.pool = pool;
    
    u8 **memory;
    if(pool)
        memory = (u8 **)get_memory(pool, sizeof(u8 *) * capacity);
    else
        memory = (u8 **)malloc(sizeof(u8 *) * capacity);
    ASSERT(memory);
    array.data = memory;
    array.capacity = capacity;
    
    return array;
}

PointerArray make_default_pointer_array()
{
    return make_pointer_array(8, NULL);
}

void reserve_pointers(PointerArray *array, s64 amount)
{
    if(array->capacity < amount)
    {
        u8 **memory;
        if(array->pool)
            memory = (u8 **)get_more_memory(array->pool, (u8 *)array->data, sizeof(u8 *) * amount, sizeof(u8 *) * array->capacity);
        else
            memory = (u8 **)realloc(array->data, sizeof(u8 *) * amount);
        ASSERT(memory);
        array->data = memory;
        array->capacity = amount;
    }
}

void add_pointer(PointerArray *array, u8 *ptr)
{
    if(array->count >= array->capacity)
    {
        reserve_pointers(array, 2*array->capacity);
    }
    
    array->data[array->count++] = ptr;
}

u8 *pop_pointer(PointerArray *array) {
    if(array->count == 0) {
        ASSERT(0);
        return NULL;
    }

    u8 *result = array->data[array->count-1];
    array->count -= 1;

    return result;
}

#endif