#ifndef QUEUE_H
#define QUEUE_H

#include "pool.h

typedef struct Queue
{
    u8 **elements;
    s64 first_index;
    s64 count;
    s64 capacity;
} Queue;

Queue make_queue(u8 **elements, s64 capacity)
{
    Queue result;
    result.elements = elements;
    result.first_index = 0;
    result.count = 0;
    result.capacity = capacity;
    return result;
}

b8 queue(Queue *que, u8 *token)
{
    if(que->count >= que->capacity)  return 0;
    que->elements[(que->first_index + que->count++) % que->capacity] = token;
    return 1;
}

u8 *dequeue(Queue *que)
{
    u8 *token;
    if(que->count <= 0)
        token = NULL;
    else
    {
        token = que->elements[que->first_index];
        que->first_index = (que->first_index + 1) % que->capacity;
        --que->count;
    }
    return token;
}

#endif