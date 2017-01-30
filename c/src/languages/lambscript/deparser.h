void deparse_simple(Delexer delexer, Writer *writer, Global ast)
{
    if(ast.type == Global_space)
        deparse_space(delexer, writer, ast.space);
}