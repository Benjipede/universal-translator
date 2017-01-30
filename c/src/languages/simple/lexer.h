#define STRING_CAPACITY 0x10000
u8 string_buffer[STRING_CAPACITY];
u8 *string_storage = string_buffer;

Token lex_simple(Reader *reader)
{
    Token token;
    u32 c;
    
    c = reader->curr(reader);
    ASSERT(c != '\r');
    switch(c)
    {
        case eof:
        {
            token.type = Token_eof;
        } break;
        case '\n':
        case ' ':
        {
            token = lex_whitespace(reader);
        } break;
        case '#':
        {
            token.type = Token_comment;
            token.comment_text.data = string_storage;
            for(c = reader->next(reader); ; c = reader->next(reader))
            {
                if(c == eof || c == '\n')
                {
                    token.comment_type = Comment_single;
                    break;
                }
                if(c == '#')
                {
                    token.comment_type = Comment_multi;
                    reader->next(reader);
                    break;
                }
                *string_storage = (u8)c;
                ++string_storage;
            }
            token.comment_text.count = string_storage - token.comment_text.data;
        } break;
        default:
        {
            token.type = Token_unknown;
            token.text.data = string_storage;
            for(c = reader->next(reader); c == '\n' || c == ' ' || c == '#'; c = reader->next(reader))
            {
                ++string_storage;
            }
            token.text.count = string_storage - token.text.data;
        }
    }
    return token;
}