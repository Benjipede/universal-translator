#ifndef POOL_H
#define POOL_H

// inspired by the pool implemented in JAI by Jonathan Blow

#include "pointer_array.h"

#include <memory.h>

#define POOL_BUCKET_SIZE_DEFAULT 65536
#define POOL_OUT_OF_BAND_SIZE_DEFAULT 6554

struct Pool
{
    s64 memblock_size;
    s64 out_of_band_size;
    u64  alignment;

    PointerArray unused_memblocks;
    PointerArray used_memblocks;
    PointerArray out_of_band_allocations;

    u8 *current_memblock;
    u8 *current_pos;
    s64 bytes_left;
};

typedef struct Pool Pool;

Pool make_pool(s64 memblock_size, s64 out_of_band_size, u64 alignment)
{
    Pool pool;
    pool.memblock_size = memblock_size;
    pool.out_of_band_size = out_of_band_size;
    pool.alignment = alignment;
    
    pool.unused_memblocks = make_default_pointer_array();
    pool.used_memblocks = make_default_pointer_array();
    pool.out_of_band_allocations = make_default_pointer_array();
    
    pool.bytes_left = 0;
    
    return pool;
}

Pool make_default_pool()
{
    return make_pool(POOL_BUCKET_SIZE_DEFAULT, POOL_OUT_OF_BAND_SIZE_DEFAULT, 8);
}

void cycle_new_block(Pool *pool)
{
    if(pool->current_memblock) {
        add_pointer(&pool->used_memblocks, pool->current_memblock);
    }

    u8 *new_block;
    if(pool->unused_memblocks.count) {
        new_block = pop_pointer(&pool->unused_memblocks);
    } else {
        new_block = (u8 *)malloc(pool->memblock_size);
    }

    pool->bytes_left = pool->memblock_size;
    pool->current_pos = new_block;
    pool->current_memblock = new_block;
}

void *get_memory_align(Pool *pool, s64 nbytes, u64 alignment)
{
    u8 *result;
    s64 alignment_padding = (u64)pool->current_pos % pool->alignment;
    
    pool->current_pos += alignment_padding;
    pool->bytes_left  -= alignment_padding;

    if(nbytes >= pool->out_of_band_size) {
        u8 *memory = (u8 *)malloc(pool->memblock_size);
        if(memory)
            add_pointer(&pool->out_of_band_allocations, memory);
        return memory;
    }
    
    if(pool->bytes_left < nbytes) {
        cycle_new_block(pool);
        if(!pool->current_memblock)
            return NULL;
    }

    result = pool->current_pos;
    pool->current_pos += nbytes;
    pool->bytes_left  -= nbytes;

    return result;
}

void *get_memory(Pool *pool, s64 nbytes)
{
    return get_memory_align(pool, nbytes, pool->alignment);
}

// Concatenation is efficient
void *get_more_memory_align(Pool *pool, void *old_memory, s64 nbytes, s64 old_nbytes, u64 alignment)
{
    if((u8 *)old_memory + old_nbytes == pool->current_pos)
    {
        s64 difference = nbytes - old_nbytes;
        if(pool->current_memblock <= (u8 *)old_memory && difference <= pool->bytes_left)
        {
            pool->current_pos += difference;
            pool->bytes_left  -= difference;
            return old_memory;
        }
    }
    
    {
        void *result = get_memory_align(pool, nbytes, alignment);
        if(result)
            memcpy(result, old_memory, old_nbytes);
        return result;
    }
}

void *get_more_memory(Pool *pool, void *old_memory, s64 nbytes, s64 old_nbytes)
{
    return get_more_memory_align(pool, old_memory, nbytes, old_nbytes, pool->alignment);
}


void reset_pool(Pool *pool)
{
    if(pool->current_memblock) {
        add_pointer(&pool->unused_memblocks, pool->current_memblock);
	    pool->current_memblock = NULL;
    }

    for(s64 index = 0; index < pool->used_memblocks.count; ++index)
        add_pointer(&pool->unused_memblocks, pool->used_memblocks.data[index]);
    pool->used_memblocks.count = 0;

    for(s64 index = 0; index < pool->out_of_band_allocations.count; ++index)
        free(pool->out_of_band_allocations.data[index]);
    pool->out_of_band_allocations.count = 0;
}


void release_pool(Pool *pool)
{
    reset_pool(pool);

    for(s64 index = 0; index < pool->unused_memblocks.count; ++index)
        free(pool->unused_memblocks.data[index]);
}



#endif