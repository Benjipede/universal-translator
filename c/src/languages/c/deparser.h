void deparse_c(Delexer delexer, Writer *writer, Global ast)
{
    Token token;
    switch(ast.type)
    {
        case Global_globals:
        {
            deparse_globals(deparse_c, delexer, writer, ast);
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
        default:
        {
            ASSERT(0)
        }
    }
}