#include <stdio.h>

void apput(Writer *writer, u32 c)
{
    if(fputc((char)c, (FILE *)writer->data) == EOF)
        perror(writer->destination);
}

void apput_more(Writer *writer, u8 *c)
{
    if(fputs((char *)c, (FILE *)writer->data) == EOF)
        perror(writer->destination);
}



Writer make_ascii_putter(char *filename)
{
    Writer writer = {0};
    FILE *file = fopen(filename, "wb");
    if(file)
    {
        writer.data = file;
        writer.destination = filename;
        writer.put = apput;
        writer.put_more = apput_more;
    }
    return writer;
}

void destroy_ascii_putter(Writer *writer)
{
    fclose((FILE *)writer->data);
}