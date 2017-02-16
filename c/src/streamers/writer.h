typedef struct Writer
{
    void *data;
    char *destination;
    
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

void put_string(Writer *writer, string text)
{
    writer->put_string(writer, text);
}

void put_cstring(Writer *writer, char *c)
{
    writer->put_more(writer, (u8 *)c);
}



void strong_put(Writer *writer, u32 c)
{
    string text;
    text.data = (u8 *)&c;
    text.count = 1;
    writer->put_string(writer, text);
}

void strong_put_more(Writer *writer, u8 *c)
{
    writer->put_string(writer, string_from_cstring((char *)c));
}

void strong_put_string(Writer *writer, string text)
{
    for(s64 index = 0; index < text.count; ++index)
    {
        writer->put(writer, text.data[index]);
    }
}


void strengthen_writer(Writer *writer)
{
    if(writer->put_string)
    {
        if(!writer->put)        writer->put = strong_put;
        if(!writer->put_more)   writer->put_more = strong_put_more;
    }
    else if(writer->put)
    {
        writer->put_string = strong_put_string;
        if(!writer->put_more)   writer->put_more = strong_put_more;
    }
    
    
}