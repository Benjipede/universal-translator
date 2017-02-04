#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

// inspired by the dynamic array implemented in JAI by Jonathan Blow

#include "pool.h"
#include "string.h"

// Set pool to NULL to use malloc as allocator
typedef struct
{
    string *data;
    s64 count;
    s64 capacity;
    
    struct Pool *pool;
} StringArray;

StringArray make_string_array(s64 capacity, struct Pool *pool)
{
    StringArray array;
    array.count = 0;
    array.capacity = 0;
    array.pool = pool;
    
    string *memory;
    if(pool)
        memory = (string *)get_memory(pool, sizeof(string ) * capacity);
    else
        memory = (string *)malloc(sizeof(string ) * capacity);
    ASSERT(memory);
    array.data = memory;
    array.capacity = capacity;
    
    return array;
}

StringArray make_default_string_array()
{
    return make_string_array(8, NULL);
}

void reserve_strings(StringArray *array, s64 amount)
{
    if(array->capacity < amount)
    {
        string *memory;
        if(array->pool)
            memory = (string *)get_more_memory(array->pool, (u8 *)array->data, sizeof(string) * amount, sizeof(string ) * array->capacity);
        else
            memory = (string *)realloc(array->data, sizeof(string ) * amount);
        ASSERT(memory);
        array->data = memory;
        array->capacity = amount;
    }
}

void add_string(StringArray *array, string text)
{
    if(array->count >= array->capacity)
    {
        reserve_strings(array, 2*array->capacity);
    }
    
    array->data[array->count++] = text;
}

string pop_string(StringArray *array) {
    if(array->count == 0) {
        string empty = {0, 0};
        ASSERT(0);
        return empty;
    }

    string result = array->data[array->count-1];
    array->count -= 1;

    return result;
}

#endif