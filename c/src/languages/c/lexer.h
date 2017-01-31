Token lex_lambscript(Reader *reader, string *storage)
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
        case '//': token.comment.type = Comment_single;
        case '/*':
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
            token.type = Token_unknown;
            token.text.data = storage->data;
            for(c = reader->next(reader); c == '\n' || c == ' ' || c == '#'; c = reader->next(reader))
            {
                ++storage->data;
                --storage->count;
            }
            token.text.count = storage->data - token.text.data;
        }
    }
    return token;
}