#include <stdio.h>

#define DATA ((adData *)reader->data)
#define CURRENT DATA->current
#define FIRST DATA->first
#define LAST DATA->last

typedef struct
{
    u8 *first;
    u8 *current;
    u8 *last;
} adData;

u32 adcurr(Reader *reader)
{
    if(CURRENT < FIRST)
        return sof;
    if(CURRENT > LAST)
        return eof;
    return (u32)*CURRENT;
}

u32 adnext(Reader *reader)
{
    
    if(CURRENT >= LAST)
    {
        CURRENT = LAST + 1;
        return eof;
    }
    ++CURRENT;
    return (u32)*CURRENT;
}

u32 adprev(Reader *reader)
{
    if(CURRENT <= FIRST)
    {
        CURRENT = FIRST - 1;
        return sof;
    }
    ++CURRENT;
    return (u32)*CURRENT;
}

u32 admove(Reader *reader, s64 amount)
{
    CURRENT += amount;
    if(CURRENT < FIRST)
    {
        CURRENT = FIRST - 1;
        return sof;
    }
    if(CURRENT > LAST)
    {
        CURRENT = LAST + 1;
        return eof;
    }
    return (u32)*CURRENT;
}

u32 adpeek(Reader *reader, s64 count)
{
    u8 *destination = CURRENT + count;
    if(destination < FIRST)
        return sof;
    else if(destination > LAST)
        return eof;
    else
        return (u32)*destination;
}

Reader make_ascii_dumper_open(FILE *file, Pool *pool)
{
    
    Reader reader;
    reader.curr = adcurr;
    reader.next = adnext;
    reader.prev = adprev;
    reader.move = admove;
    reader.peek = adpeek;
    strengthen_reader(&reader);
    
    {
        u64 filesize;
        fseek(file, 0, SEEK_END);
        filesize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        reader.data = get_memory(pool, sizeof(adData));
        ((adData *)reader.data)->first = get_memory_align(pool, filesize, 1);
        ((adData *)reader.data)->current = ((adData *)reader.data)->first;
        ((adData *)reader.data)->last = ((adData *)reader.data)->first + fread(((adData *)reader.data)->first, 1, filesize, file) - 1;
    }
    
    return reader;
}

Reader make_ascii_dumper(char *filename, Pool *pool)
{
    Reader reader = {0};
    FILE *file = fopen(filename, "rb");
    if(file)
    {
        reader = make_ascii_dumper_open(file, pool);
        fclose(file);
    }
    return reader;
}

#undef DATA
#undef CURRENT
#undef FIRST
#undef LAST