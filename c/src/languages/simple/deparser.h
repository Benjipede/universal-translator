void deparse_simple(Delexer delexer, Writer *writer, Global ast)
{
    Token token;
    switch(ast.type)
    {
        case Global_globals:
        {
            deparse_globals(deparse_simple, delexer, writer, ast);
        } break;
        case Global_unsupported:
        {
            token.type = Token_unsupported;
            token.text = ast.text;
            delexer(writer, token);
        } break;
        case Global_unknown_token:
        {
            token.type = Token_unknown;
            token.text = ast.text;
            delexer(writer, token);
        } break;
        case Global_space:
        {
            deparse_space(delexer, writer, ast.space);
        } break;
        case Global_expression:
        {
            if(ast.expression.type == Expression_variable)
            {
                token.type = Token_identifier;
                token.text = ast.expression.text;
                delexer(writer, token);
            }
            else
            {
                token.type = Token_unsupported;
                token.text = ast.text;
                delexer(writer, token);
            }
        } break;
        default:
        {
            ASSERT(0)
        }
    }
}