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

static
u32 adcurr(Reader *reader)
{
    if(CURRENT < FIRST)
        return sof;
    if(CURRENT > LAST)
        return eof;
    return (u32)*CURRENT;
}

static
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

static
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

static
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

static
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

static
void read_entire_file_into_memory(char *filename, adData *result)
{
    FILE *file = fopen(filename, "rb");
    if(file)
    {
        fseek(file, 0, SEEK_END);
        s64 filesize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        result->current = result->first = (u8 *)malloc(filesize);
        result->last = result->first + fread(result->first, 1, filesize, file) - 1;
        
        fclose(file);
    }
}

Reader make_ascii_dumper(char *filename, adData *data)
{
    Reader reader;
    reader.curr = adcurr;
    reader.next = adnext;
    reader.prev = adprev;
    reader.move = admove;
    reader.peek = adpeek;
    strengthen_reader(&reader);
    
    reader.data = data;
    read_entire_file_into_memory(filename, reader.data);
    
    return reader;
}

void destroy_ascii_dumper(Reader *reader)
{
    free(FIRST);
    //free(reader->data); //add this when make_ascii_dumper takes an allocator
}

#undef DATA
#undef CURRENT
#undef FIRST
#undef LAST