Token *lex_whitespace(Reader *reader, Pool *pool)
{
    TokenWhitespace *token = (TokenWhitespace *)get_memory(pool, sizeof(TokenWhitespace));
    u32 c = curr(reader);
    
    ((Token *)token)->kind = Token_whitespace;
    token->line_feed_count = 0;
    token->space_count = 0;
    while(c == ' ')
    {
        ++token->space_count;
        c = next(reader);
    }
    while(c == '\n')
    {
        token->space_count = 0;
        ++token->line_feed_count;
        c = next(reader);
        while(c == ' ')
        {
            ++token->space_count;
            c = next(reader);
        }
    }
    
    return (Token *)token;
}

Token *lex_identifier(Reader *reader, Pool *pool)
{
    Token *token = (Token *)get_memory(pool, sizeof(TokenText));
    token->kind = Token_identifier;
    string *text = &((TokenText *)token)->text;
    text->count = 0;
    text->data = get_memory_align(pool, 0, 1);
    u32 c = curr(reader);
    
    if(is_alpha(c) || c == '_')
    {
        append_to_string(text, c, pool);
        
        for(c = reader->next(reader); is_alphanumeric(c) || c == '_'; c = reader->next(reader))
        {
            append_to_string(text, c, pool);
        }
    }
    
    return (Token *)token;
}

Token *lex_unknown(Reader *reader, Pool *pool, b8 (*still_unknown)(Reader *, Pool *))
{
    TokenText *token = (TokenText *)get_memory(pool, sizeof(TokenText));
    ((Token *)token)->kind = Token_unknown;
    token->text.count = 0;
    token->text.data = (u8 *)get_memory_align(pool, 0, 1);
    
    for(u32 c = curr(reader); c != eof && c != '\n' && c != ' ' && still_unknown(reader, pool); c = next(reader))
    {
        append_to_string(&token->text, c, pool);
    }
    return (Token *)token;
}