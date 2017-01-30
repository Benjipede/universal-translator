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