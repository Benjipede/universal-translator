void apput(Writer *writer, u32 c)
{
    fputc((char)c, (FILE *)writer->data);
}

Writer make_ascii_putter(char *filename)
{
    Writer writer;
    writer.put = apput;
    writer.data = fopen(filename, "wb");
    return writer;
}

void destroy_ascii_putter(Writer *writer)
{
    fclose((FILE *)writer->data);
}