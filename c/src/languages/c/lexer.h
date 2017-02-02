b8 still_unknown_c(Reader *reader, Pool *pool)
{
    u32 c;
    c = curr(reader);
    return !is_alpha(c) && c != '_' && c != ';' && c != '/';
}

SUPPORT(DELIMITER, line comment, multiline comment, identifier, semicolon, DELIMITER)
Token lex_c(Reader *reader, Pool *pool)
{
    Token token;
    u32 c;
    
    c = curr(reader);
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
        case '/':
        {
            c = next(reader);
            switch(c)
            {
                case '/':
                {
                    token.type = Token_comment;
                    token.comment.type = Comment_single;
                    token.comment.text.count = 0;
                    token.comment.text.data = get_memory_align(pool, 0, 1);
                    
                    for(c = next(reader); ; c = next(reader))
                    {
                        if(c == eof || c == '\n')
                            break;
                        append_to_string(&token.comment.text, c, pool);
                    }
                } break;
                case '*':
                {
                    token.type = Token_comment;
                    token.comment.type = Comment_multi;
                    token.comment.text.count = 0;
                    token.comment.text.data = get_memory_align(pool, 0, 1);
                    for(c = next(reader); ; c = next(reader))
                    {
                        if(c == eof) // How should lexical errors be handled?
                        {
                            token.type = Token_error;
                            token.text = string_from_cstring("File ended before multiline comment was closed.");
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
                            append_to_string(&token.comment.text, '*', pool);
                        }
                        append_to_string(&token.comment.text, c, pool);
                    }
                } break;
                default:
                {
                    token.type = Token_unsupported;
                    token.text = string_from_cstring("Division is not yet supported.");
                }
            }
        } break;
        case ';':
        {
            next(reader);
            token.type = Token_semicolon;
        } break;
        default:
        {
            if(is_alpha(c) || c == '_')
            {
                token.type = Token_identifier;
                token.text.count = 0;
                token.text.data = get_memory_align(pool, 0, 1);
                
                append_to_string(&token.comment.text, c, pool);
                
                for(c = next(reader); is_alphanumeric(c) || c == '_'; c = next(reader))
                    append_to_string(&token.comment.text, c, pool);
            }
            else
            {
                token = lex_unknown(reader, pool, still_unknown_c);
            }
        }
    }
    return token;
}