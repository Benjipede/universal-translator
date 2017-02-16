b8 still_unknown_c(Reader *reader, Pool *pool)
{
    u32 c;
    c = curr(reader);
    return !is_alpha(c) && c != '_' && c != ';' && c != '/';
}

Token *lex_c(Reader *reader, Pool *pool)
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
        case '/':
        {
            c = next(reader);
            switch(c)
            {
                case '/':
                {
                    token = (Token *)get_memory(pool, sizeof(TokenComment));
                    token->kind = Token_comment;
                    ((TokenComment *)token)->type = Comment_single;
                    {
                        string *text = &((TokenText *)token)->text;
                        text->count = 0;
                        text->data = get_memory_align(pool, 0, 1);
                    
                        for(c = next(reader); ; c = next(reader))
                        {
                            if(c == eof || c == '\n')
                                break;
                            append_to_string(text, c, pool);
                        }
                    }
                } break;
                case '*':
                {
                    token = (Token *)get_memory(pool, sizeof(TokenComment));
                    token->kind = Token_comment;
                    ((TokenComment *)token)->type = Comment_multi;
                    {
                        string *text = &((TokenText *)token)->text;
                        text->count = 0;
                        text->data = get_memory_align(pool, 0, 1);
                        
                        for(c = next(reader); ; c = next(reader))
                        {
                            if(c == eof) // How should lexical errors be handled?
                            {
                                token->kind = Token_error;
                                *text = string_from_cstring("File ended before multiline comment was closed.");
                                break;
                            }
                            if(c == '*')
                            {
                                c = next(reader);
                                if(c == '/')
                                {
                                    next(reader);
                                    break;
                                }                                
                                append_to_string(text, '*', pool);
                            }
                            append_to_string(text, c, pool);
                        }
                    }
                } break;
                default:
                {
                    token = (Token *)get_memory(pool, sizeof(Token));
                    token->kind = '/';
                }
            }
        } break;
        case ';':
        {
            token = (Token *)get_memory(pool, sizeof(Token));
            token->kind = ';';
            next(reader);
        } break;
        default:
        {
            if(is_alpha(c) || c == '_')
            {
                token = lex_identifier(reader, pool);
            }
            else
            {
                token = lex_unknown(reader, pool, still_unknown_c);
            }
        }
    }
    return token;
}