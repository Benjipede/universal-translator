void delex_simple(Writer *writer, Token token)
{
    switch(token.type)
    {
        case Token_none:
        case Token_sof:
        case Token_eof:
        break;
        case Token_whitespace:
        {
            delex_whitespace(writer, token);
        } break;
        case Token_unsupported:
        {
            put_cstring(writer, "# @Unsupported: ");
            for(s64 index = 0; index < token.text.count; ++index)
            {
                put(writer, token.text.data[index]);
            }
            put_cstring(writer, " #");
        } break;
        case Token_unknown:
        {
            put_cstring(writer, "# @Unknown token: ");
            for(s64 index = 0; index < token.text.count; ++index)
            {
                put(writer, token.text.data[index]);
            }
            put_cstring(writer, " #");
        } break;
        case Token_comment:
        {
            writer->put(writer, '#');
            for(s64 index = 0; index < token.comment.text.count; ++index)
            {
                put(writer, token.comment.text.data[index]);
            }
            if(token.comment.type == Comment_multi)
                put(writer, '#');
        } break;
        case Token_identifier:
        {
            for(s64 index = 0; index < token.text.count; ++index)
            {
                put(writer, token.text.data[index]);
            }
        } break;
        default:
        {
            ASSERT(0)
        }
    }
}