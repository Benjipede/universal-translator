void deparse_c(Delexer delexer, Writer *writer, Global ast)
{
    switch(ast.type)
    {
        case Global_globals:
        {
            deparse_globals(deparse_c, delexer, writer, ast);
        } break;
        case Global_unsupported:
        {
            TokenText token;
            token.self.kind = Token_unsupported;
            token.text = ast.text;
            delexer(writer, (Token *)&token);
        } break;
        case Global_unknown_token:
        {
            TokenText token;
            token.self.kind = Token_unknown;
            token.text = ast.text;
            delexer(writer, (Token *)&token);
        } break;
        case Global_space:
        {
            deparse_space(delexer, writer, ast.space);
        } break;
        case Global_expression:
        {
            if(ast.expression.type == Expression_variable)
            {
                TokenText token;
                token.self.kind = Token_identifier;
                token.text = ast.expression.text;
                delexer(writer, (Token *)&token);
                token.self.kind = ';';
                delexer(writer, (Token *)&token);
            }
            else
            {
                TokenText token;
                token.self.kind = Token_unsupported;
                token.text = ast.text;
                delexer(writer, (Token *)&token);
            }
        } break;
        default:
        {
            ASSERT(0)
        }
    }
}