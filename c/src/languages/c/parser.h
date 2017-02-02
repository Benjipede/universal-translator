Global parse_c(Lexer lexer, Reader *reader, Pool *pool, Stack *stack, Queue *que)
{
    Global ast = {0};
    Token token;
    
    token = pop(stack);
    if(token.type == Token_none)
        token = lexer(reader, pool);
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
            ast.space = parse_space(lexer, reader, pool, stack, que);
        } break;
        case Token_identifier:
        {
            Token token_next;
            token_next = lexer(reader, pool);
            if(token_next.type == Token_semicolon)
            {
                ast.type = Global_expression;
                ast.expression.type = Expression_variable;
                ast.expression.text = token.text;
            }
            else
            {
                ast.type = Global_unsupported;
                ast.text = string_from_cstring("Only variables are supported as top-level expressions.");
            }
        } break;
        default:
        {
            ASSERT(0)
        }
    }
    return ast;
}