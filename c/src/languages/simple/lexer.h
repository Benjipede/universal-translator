b8 still_unknown_simple(Reader *reader, Pool *pool)
{
    u32 c;
    c = curr(reader);
    return !is_alpha(c) && c != '_' && c != '#';
}

Token lex_simple(Reader *reader, Pool *pool)
{
    Token token;
    u32 c;
    
    c = reader->curr(reader);
    switch(c)
    {
        case eof:
        {
            token.type = Token_eof;
        } break;
        case sof:
        {
            token.type = Token_sof;
        } break;
        case '\n':
        case ' ':
        {
            token = lex_whitespace(reader);
        } break;
        case '#':
        {
            token.type = Token_comment;
            token.comment.text.count = 0;
            token.comment.text.data = get_memory_align(pool, 0, 1);
            
            for(c = reader->next(reader); ; c = reader->next(reader))
            {
                if(c == eof || c == '\n')
                {
                    token.comment.type = Comment_single;
                    break;
                }
                if(c == '#')
                {
                    token.comment.type = Comment_multi;
                    reader->next(reader);
                    break;
                }
                append_to_string(&token.comment.text, c, pool);
            }
        } break;
        default:
        {
            if(is_alpha(c) || c == '_')
            {
                token.type = Token_identifier;
                token.comment.text.count = 0;
                token.comment.text.data = get_memory_align(pool, 0, 1);
                
                append_to_string(&token.comment.text, c, pool);
                
                for(c = reader->next(reader); is_alphanumeric(c) || c == '_'; c = reader->next(reader))
                {
                    append_to_string(&token.comment.text, c, pool);
                }
            }
            else
            {
                token = lex_unknown(reader, pool, still_unknown_simple);
            }
        }
    }
    return token;
}