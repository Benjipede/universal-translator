b8 still_unknown_simple(Reader *reader, Pool *pool)
{
    u32 c;
    c = curr(reader);
    return !is_alpha(c) && c != '_' && c != '#';
}

Token *lex_simple(Reader *reader, Pool *pool)
{
    Token *token;
    u32 c;
    
    c = curr(reader);
    switch(c)
    {
        case eof:
        {
            token = (Token *)get_memory(pool, sizeof(Token));
            token->kind = Token_eof;
        } break;
        case sof:
        {
            token = (Token *)get_memory(pool, sizeof(Token));
            token->kind = Token_sof;
        } break;
        case '\n':
        case ' ':
        {
            token = lex_whitespace(reader, pool);
        } break;
        case '#':
        {
            token = (Token *)get_memory(pool, sizeof(TokenComment));
            token->kind = Token_comment;
            {
                string *text = &((TokenText *)token)->text;
                text->count = 0;
                text->data = get_memory_align(pool, 0, 1);
            
                for(c = reader->next(reader); ; c = reader->next(reader))
                {
                    if(c == eof || c == '\n')
                    {
                        ((TokenComment *)token)->type = Comment_single;
                        break;
                    }
                    if(c == '#')
                    {
                        ((TokenComment *)token)->type = Comment_multi;
                        reader->next(reader);
                        break;
                    }
                    append_to_string(text, c, pool);
                }
            }
        } break;
        default:
        {
            if(is_alpha(c) || c == '_')
            {
                token = lex_identifier(reader, pool);
            }
            else
            {
                token = lex_unknown(reader, pool, still_unknown_simple);
            }
        }
    }
    return token;
}