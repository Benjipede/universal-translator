#ifndef QUEUE_H
#define QUEUE_H

#include "pool.h"

typedef struct Queue
{
    PointerArray array;
    s64 first_index;
} Queue;

Queue make_queue(s64 capacity, Pool *pool)
{
    Queue result;
    result.array = make_pointer_array(capacity, pool);
    result.first_index = 0;
    return result;
}

void queue(Queue *que, u8 *ptr)
{
    if(que->array.count >= que->array.capacity)
    {
        s64 old_capacity = que->array.capacity;
        reserve_pointers(&que->array, que->array.capacity);
        if(que->first_index)
        {
            s64 diff_capacity = que->array.capacity - old_capacity;
            memcpy(que->array.data + old_capacity, que->array.data, diff_capacity);
            memcpy(que->array.data, que->array.data + diff_capacity, que->first_index + que->array.count - que->array.capacity);
        }
    }
    que->array.data[(que->first_index + que->array.count++) % que->array.capacity] = ptr;
}

u8 *dequeue(Queue *que)
{
    u8 *result;
    if(que->array.count <= 0)
        result = NULL;
    else
    {
        result = que->array.data[que->first_index];
        que->first_index = (que->first_index + 1) % que->array.capacity;
        --que->array.count;
    }
    return result;
}

#endif