#include <stdio.h>

void apput(Writer *writer, u32 c)
{
    fputc((char)c, (FILE *)writer->data);
}

void apput_more(Writer *writer, u8 *c)
{
    fputs((char *)c, (FILE *)writer->data);
}

Writer make_ascii_putter_open(FILE *file)
{
    Writer writer;
    writer.put = apput;
    writer.put_more = apput_more;
    writer.data = file;
    return writer;
}

Writer make_ascii_putter(char *filename)
{
    Writer writer = {0};
    FILE *file = fopen(filename, "wb");
    if(file)
    {
        writer = make_ascii_putter_open(file);
        fclose(file);
    }
    return writer;
}

void destroy_ascii_putter(Writer *writer)
{
    fclose((FILE *)writer->data);
}