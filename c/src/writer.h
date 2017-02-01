typedef struct Writer
{
    void *data;
    
    void (*put)(struct Writer *, u32);
    void (*put_more)(struct Writer *, u8 *);
    void (*put_string)(struct Writer *, string);
} Writer;

void put(Writer *writer, u32 c)
{
    writer->put(writer, c);
}

void put_more(Writer *writer, u8 *c)
{
    writer->put_more(writer, c);
}

void put_cstring(Writer *writer, char *c)
{
    writer->put_more(writer, (u8 *)c);
}

void strong_put_more(Writer *writer, u8 *c)
{
    for(u8 *d = c; *d; ++d)
    {
        writer->put(writer, *d);
    }
}

#include "writers/ascii_putter.h"