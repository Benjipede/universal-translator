typedef struct Writer
{
    void *data;
    
    void (*put)(struct Writer *, u32);
} Writer;

void put(Writer *writer, u32 c)
{
    writer->put(writer, c);
}

void put_c_string(Writer *writer, char *c)
{
    for(char *d = c; *d; ++d)
    {
        writer->put(writer, *d);
    }
}

#include "writers/ascii_putter.h"