void delex_simple(Writer *writer, Token *token)
{
    switch(token->kind)
    {
        case Token_none:
        case Token_sof:
        case Token_eof:
        break;
        case Token_whitespace:
        {
            delex_whitespace(writer, (TokenWhitespace *)token);
        } break;
        case Token_unsupported:
        {
            put_cstring(writer, "# @Unsupported: ");
            put_string(writer, ((TokenText *)token)->text);
            put_cstring(writer, " #");
        } break;
        case Token_unknown:
        {
            put_cstring(writer, "# @Unknown: ");
            put_string(writer, ((TokenText *)token)->text);
            put_cstring(writer, " #");
        } break;
        case Token_comment:
        {
            writer->put(writer, '#');
            put_string(writer, ((TokenText *)token)->text);
            if(((TokenComment *)token)->type == Comment_multi)
                put(writer, '#');
        } break;
        case Token_identifier:
        {
            put_string(writer, ((TokenText *)token)->text);
        } break;
        default:
        {
            ASSERT(0)
        }
    }
}