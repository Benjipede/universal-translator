void delex_simple(Writer *writer, Token token)
{
    switch(token.type)
    {
        string text;
        case Token_none:
        case Token_sof:
        case Token_eof:
        break;
        case Token_whitespace:
        {
            delex_whitespace(writer, token);
        } break;
        case Token_unknown:
        token.type = Comment_multi;
        text = token.text;
        token.comment_text = text;
        case Token_comment:
        {
            writer->put(writer, '#');
            for(s64 index = 0; index < token.comment_text.count; ++index)
            {
                writer->put(writer, token.comment_text.data[index]);
            }
            if(token.comment_type == Comment_multi)
                writer->put(writer, '#');
        } break;
        default:
        {
            //??
        }
    }
}