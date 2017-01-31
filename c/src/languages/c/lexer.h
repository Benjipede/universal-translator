b8 still_unknown_c(Reader *reader, string *storage)
{
    u32 c;
    c = curr(reader);
    return c != ';' && c != '/';
}

Token lex_c(Reader *reader, string *storage)
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
        case '/':
        {
            c = next(reader);
            switch(c)
            {
                case '/':
                {
                    token.type = Token_comment;
                    token.comment.type = Comment_single;
                    token.comment.text.data = storage->data;
                    for(c = reader->next(reader); ; c = reader->next(reader))
                    {
                        if(c == eof || c == '\n')
                        {
                            token.comment.type = Comment_single;
                            break;
                        }
                        *storage->data = (u8)c;
                        ++storage->data;
                        --storage->count;
                    }
                    token.comment.text.count = storage->data - token.comment.text.data;
                } break;
                case '*':
                {
                    string *token_text;
                    token.type = Token_comment;
                    token.comment.type = Comment_multi;
                    { // in case of an error
                        storage->data[0] = '/';
                        storage->data[1] = '*';
                        storage->data += 2;
                    }
                    token.comment.text.data = storage->data;
                    for(c = reader->next(reader); ; c = reader->next(reader))
                    {
                        if(c == eof) // How should lexical errors be handled?
                        {
                            token.type = Token_error;
                            string temp = token.comment.text;
                            temp.data -= 2;
                            token.text = temp;
                            token_text = &token.text;
                            break;
                        }
                        if(c == '*')
                        {
                            c = next(reader);
                            if(c == '/')
                            {
                                reader->next(reader);
                                token_text = &token.comment.text;
                                break;
                            }                                
                            *storage->data = '*';
                            ++storage->data;
                            --storage->count;
                        }
                        *storage->data = (u8)c;
                        ++storage->data;
                        --storage->count;
                    }
                    token_text->count = storage->data - token_text->data;
                } break;
                default:
                {
                    token.type = Token_unsupported;
                    token.text = string_from_c_string("Division is not yet supported.");
                }
            }
        } break;
        default: token = lex_unknown(reader, storage, still_unknown_c);
    }
    return token;
}