b8 still_unknown_simple(Reader *reader, string *storage)
{
    u32 c;
    c = curr(reader);
    return c != '#';
}

Token lex_simple(Reader *reader, string *storage)
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
            token.comment.text.data = storage->data;
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
                *storage->data = (u8)c;
                ++storage->data;
                --storage->count;
            }
            token.comment.text.count = storage->data - token.comment.text.data;
        } break;
        default:
        {
            if(is_alpha(c) || c == '_')
            {
                token.type = Token_identifier;
                token.text.data = storage->data;
                
                *storage->data = (u8)c;
                ++storage->data;
                --storage->count;
                
                for(c = reader->next(reader); is_alphanumeric(c) || c == '_'; c = reader->next(reader))
                {
                    *storage->data = (u8)c;
                    ++storage->data;
                    --storage->count;
                }
                token.text.count = storage->data - token.text.data;
            }
            else
            {
                token = lex_unknown(reader, storage, still_unknown_simple);
            }
        }
    }
    return token;
}