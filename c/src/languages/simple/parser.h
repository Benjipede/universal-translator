Global parse_simple(Lexer lexer, Reader *reader, Stack *stack, Queue *que)
{
    Global ast;
    ast = parse_global_space(lexer, reader, stack, que);
    return ast;
}