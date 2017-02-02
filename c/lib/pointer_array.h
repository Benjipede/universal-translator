#ifndef POINTER_ARRAY_H
#define POINTER_ARRAY_H

#include "nicetypes.h"
#include "misc.h"

#include <stdlib.h>

typedef struct
{
    u8 **data;
    s64 count;
    s64 capacity;
} PointerArray;

void reserve_pointers(PointerArray *array, s64 amount)
{
    if(array->capacity < amount)
    {
        u8 **memory = (u8 **)realloc(array->data, amount);
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

PointerArray make_pointer_array(s64 capacity)
{
    PointerArray result;
    result.count = 0;
    result.capacity = 0;
    reserve_pointers(&result, capacity);
    return result;
}

#endif