Global parse_simple(Lexer lexer, Reader *reader, string *storage, Stack *stack, Queue *que)
{
    Global ast = {0};
    Token token;
    
    token = pop(stack);
    if(token.type == Token_none)
        token = lexer(reader, storage);
    switch(token.type)
    {
        case Token_eof:
        {
            ast.type = Global_eof;
        } break;
        case Token_unknown:
        {
            ast.type = Global_unknown_token;
            ast.text = token.text;
        } break;
        case Token_whitespace:
        case Token_comment:
        {
            push(stack, token);
            ast.type = Global_space;
            ast.space = parse_space(lexer, reader, storage, stack, que);
        } break;
        case Token_identifier:
        {
            ast.type = Global_expression;
            ast.expression.type = Expression_variable;
            ast.expression.text = token.text;
        } break;
        default:
        {
            ASSERT(0)
        }
    }
    return ast;
}