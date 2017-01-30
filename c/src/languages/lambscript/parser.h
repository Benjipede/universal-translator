Global parse_lambscript(Lexer lexer, Reader *reader, string *storage, Stack *stack, Queue *que)
{
    Global ast;
    ast = parse_global_space(lexer, reader, storage, stack, que);
    return ast;
}