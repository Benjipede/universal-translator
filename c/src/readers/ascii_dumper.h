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
/*
static
void *read_entire_file_and_create_addata(char *filename, string *storage)
{
    adData *result;
    FILE *file = fopen(filename, "rb");
    if(file)
    {
        s64 filesize, storage_used;
        
        fseek(file, 0, SEEK_END);
        filesize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        storage_used = filesize + sizeof(adData);
        ASSERT(storage_used <= storage->count);
        fread(storage->data, 1, filesize, file);
        
        result = (adData *)(storage->data + filesize);
        result->current = result->first = storage->data;
        result->last = result->first + filesize - 1;
        
        storage->count -= storage_used;
        storage->data  += storage_used;
        
        fclose(file);
    }
    return result;
}
*/
Reader make_ascii_dumper(char *filename, string *storage)
{
    Reader reader;
    reader.curr = adcurr;
    reader.next = adnext;
    reader.prev = adprev;
    reader.move = admove;
    reader.peek = adpeek;
    strengthen_reader(&reader);
    
    {
        FILE *file = fopen(filename, "rb");
        if(file)
        {
            s64 filesize, storage_used;
            
            fseek(file, 0, SEEK_END);
            filesize = ftell(file);
            fseek(file, 0, SEEK_SET);
            
            storage_used = filesize + sizeof(adData);
            ASSERT(storage_used <= storage->count);
            fread(storage->data, 1, filesize, file);
            
            reader.data = (storage->data + filesize);
            ((adData *)reader.data)->current = ((adData *)reader.data)->first = storage->data;
            ((adData *)reader.data)->last = ((adData *)reader.data)->first + filesize - 1;
            
            storage->count -= storage_used;
            storage->data  += storage_used;
            
            fclose(file);
        }
    }
    
    return reader;
}

#undef DATA
#undef CURRENT
#undef FIRST
#undef LAST