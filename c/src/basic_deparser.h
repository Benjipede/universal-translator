void deparse_globals(Deparser deparser, Delexer delexer, Writer *writer, Global ast)
{
    for(s64 index = 0; index < ast.globals.count; ++index)
    {
        deparser(delexer, writer, ast.globals.elements[index]);
    }
}

void deparse_space(Delexer delexer, Writer *writer, Space space)
{
    Token token_whitespace, token_comment;
    token_whitespace.type = Token_whitespace;
    {
        token_whitespace.newline_count = space.initial_whitespace.newline_count;
        token_whitespace.space_count = space.initial_whitespace.space_count;
        delexer(writer, token_whitespace);
    }
    token_comment.type = Token_comment;
    for(s64 index = 0; index < space.count; ++index)
    {
        {
            token_comment.comment.type = space.comments[index].type;
            token_comment.comment.text = space.comments[index].text;
            delexer(writer, token_comment);
        }
        {
            token_whitespace.newline_count = space.comments[index].whitespace.newline_count;
            token_whitespace.space_count = space.comments[index].whitespace.space_count;
            delexer(writer, token_whitespace);
        }
    }
}