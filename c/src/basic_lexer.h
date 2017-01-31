Token lex_whitespace(Reader *reader)
{
    Token token;
    u32 c;
    c = reader->curr(reader);
    //if(c != '\n' && c != ' ')  c = reader->next(reader);
    
    token.type = Token_whitespace;
    token.newline_count = 0;
    token.space_count = 0;
    while(c == ' ')
    {
        ++token.space_count;
        c = reader->next(reader);
    }
    while(c == '\n')
    {
        token.space_count = 0;
        ++token.newline_count;
        c = reader->next(reader);
        while(c == ' ')
        {
            ++token.space_count;
            c = reader->next(reader);
        }
    }
    
    return token;
}

Token lex_unknown(Reader *reader, string *storage, b8 (*still_unknown)(Reader *, string *))
{
    Token token;
    u32 c;
    token.type = Token_unknown;
    token.text.data = storage->data;
    for(c = reader->next(reader); c != '\n' || c != ' '|| still_unknown(reader, storage); c = reader->next(reader))
    {
        ++storage->data;
        --storage->count;
    }
    token.text.count = storage->data - token.text.data;
    return token;
}