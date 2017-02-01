void apput(Writer *writer, u32 c)
{
    fputc((char)c, (FILE *)writer->data);
}

void apput_more(Writer *writer, u8 *c)
{
    fputs((char *)c, (FILE *)writer->data);
}

Writer make_ascii_putter(char *filename)
{
    Writer writer;
    writer.put = apput;
    writer.put_more = apput_more;
    writer.data = fopen(filename, "wb");
    return writer;
}

void destroy_ascii_putter(Writer *writer)
{
    fclose((FILE *)writer->data);
}