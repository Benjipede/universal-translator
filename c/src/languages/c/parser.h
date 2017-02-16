Global parse_c(Lexer lexer, Reader *reader, Pool *pool, Queue *bank, Queue *que)
{
    Global ast = {0};
    Token *token = get_token(lexer, reader, pool, bank);
    switch(token->kind)
    {
        case Token_eof:
        {
            ast.type = Global_eof;
        } break;
        case Token_unknown:
        {
            ast.type = Global_unknown_token;
            ast.text = ((TokenText *)token)->text;
        } break;
        case Token_whitespace:
        case Token_comment:
        {
            queue(bank, (u8 *)token);
            ast.type = Global_space;
            ast.space = parse_space(lexer, reader, pool, bank, que);
        } break;
        case Token_identifier:
        {
            Token *token_next;
            token_next = get_token(lexer, reader, pool, bank);
            if(token_next->kind == ';')
            {
                ast.type = Global_expression;
                ast.expression.type = Expression_variable;
                ast.expression.text = ((TokenText *)token)->text;
            }
            else
            {
                ast.type = Global_unsupported;
                ast.text = string_from_cstring("Only variables are supported as top-level expressions.");
            }
        } break;
        case '/':
        {
            ast.type = Global_unsupported;
            ast.text = string_from_cstring("/");
        } break;
        default:
        {
            ASSERT(0)
        }
    }
    return ast;
}