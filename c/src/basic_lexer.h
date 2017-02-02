Token lex_whitespace(Reader *reader)
{
    Token token;
    u32 c;
    c = curr(reader);
    
    token.type = Token_whitespace;
    token.newline_count = 0;
    token.space_count = 0;
    while(c == ' ')
    {
        ++token.space_count;
        c = next(reader);
    }
    while(c == '\n')
    {
        token.space_count = 0;
        ++token.newline_count;
        c = next(reader);
        while(c == ' ')
        {
            ++token.space_count;
            c = next(reader);
        }
    }
    
    return token;
}

void append_to_string(string *text, u32 c, Pool *pool)
{
    u8 size = 1; // @ascii
    if(pool->bytes_left < size)
        *text = copy_string(get_memory_align(pool, text->count + size, 1), *text);
    else
        get_memory_align(pool, size, 1);
    text->data[text->count] = (u8)c; // @ascii
    ++text->count;
}

Token lex_unknown(Reader *reader, Pool *pool, b8 (*still_unknown)(Reader *, Pool *))
{
    Token token;
    token.type = Token_unknown;
    token.text.count = 0;
    token.text.data = get_memory_align(pool, 0, 1);
    
    for(u32 c = curr(reader); c != eof && c != '\n' && c != ' ' && still_unknown(reader, pool); c = next(reader))
    {
        append_to_string(&token.text, c, pool);
    }
    return token;
}