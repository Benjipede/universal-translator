#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

// inspired by the dynamic array implemented in JAI by Jonathan Blow

#include "pool.h"

// Set pool to NULL to use malloc as allocator
typedef struct
{
    u8 *data;
    s64 count;
    s64 capacity;
    
    struct Pool *pool;
} ByteArray;

ByteArray make_byte_array(s64 capacity, struct Pool *pool)
{
    ByteArray array;
    array.count = 0;
    array.capacity = 0;
    array.pool = pool;
    
    u8 *memory;
    if(pool)
        memory = (u8 *)get_memory(pool, sizeof(u8) * capacity);
    else
        memory = (u8 *)malloc(sizeof(u8) * capacity);
    ASSERT(memory);
    array.data = memory;
    array.capacity = capacity;
    
    return array;
}

ByteArray make_default_byte_array()
{
    return make_byte_array(8, NULL);
}

void reserve_bytes(ByteArray *array, s64 amount)
{
    if(array->capacity < amount)
    {
        u8 *memory;
        if(array->pool)
            memory = (u8 *)get_more_memory(array->pool, (u8 *)array->data, sizeof(u8) * amount, sizeof(u8) * array->capacity);
        else
            memory = (u8 *)realloc(array->data, sizeof(u8) * amount);
        ASSERT(memory);
        array->data = memory;
        array->capacity = amount;
    }
}

void add_byte(ByteArray *array, u8 byte)
{
    if(array->count >= array->capacity)
    {
        reserve_bytes(array, 2*array->capacity);
    }
    
    array->data[array->count++] = byte;
}

u8 pop_byte(ByteArray *array) {
    if(array->count == 0) {
        ASSERT(0);
        return 0;
    }

    u8 result = array->data[array->count-1];
    array->count -= 1;

    return result;
}

#endif