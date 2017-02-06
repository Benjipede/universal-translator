#ifndef ARRAY_ARRAY_H
#define ARRAY_ARRAY_H

// inspired by the dynamic array implemented in JAI by Jonathan Blow

#include "pointer_array.h"

// Set pool to NULL to use malloc as allocator
typedef struct
{
    PointerArray *data;
    s64 count;
    s64 capacity;
    
    struct Pool *pool;
} ArrayArray;

ArrayArray make_array_array(s64 capacity, struct Pool *pool)
{
    ArrayArray array;
    array.count = 0;
    array.capacity = 0;
    array.pool = pool;
    
    void *memory;
    if(pool)
        memory = get_memory(pool, sizeof(PointerArray) * capacity);
    else
        memory = malloc(sizeof(PointerArray) * capacity);
    ASSERT(memory);
    array.data = (PointerArray *)memory;
    array.capacity = capacity;
    
    return array;
}

ArrayArray make_default_array_array()
{
    return make_array_array(8, NULL);
}

void reserve_arrays(ArrayArray *array, s64 amount)
{
    if(array->capacity < amount)
    {
        void *memory;
        if(array->pool)
            memory = get_more_memory(array->pool, (u8 *)array->data, sizeof(PointerArray) * amount, sizeof(PointerArray) * array->capacity);
        else
            memory = realloc(array->data, sizeof(PointerArray) * amount);
        ASSERT(memory);
        array->data = (PointerArray *)memory;
        array->capacity = amount;
    }
}

void add_array(ArrayArray *array, PointerArray arr)
{
    if(array->count >= array->capacity)
    {
        reserve_arrays(array, 2*array->capacity);
    }
    
    array->data[array->count++] = arr;
}

PointerArray pop_array(ArrayArray *array)
{
    PointerArray result;
    
    if(array->count == 0) {
        result.count = 0;
        result.capacity = 0;
        ASSERT(0);
        return result;
    }

    result = array->data[array->count-1];
    array->count -= 1;

    return result;
}

#endif