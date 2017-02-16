void deparse_globals(Deparser deparser, Delexer delexer, Writer *writer, Global ast)
{
    for(s64 index = 0; index < ast.globals.count; ++index)
    {
        deparser(delexer, writer, ast.globals.elements[index]);
    }
}

void deparse_space(Delexer delexer, Writer *writer, Space space)
{
    TokenWhitespace token_whitespace;
    TokenComment token_comment;
    token_whitespace.self.kind = Token_whitespace;
    {
        token_whitespace.line_feed_count = space.initial_whitespace.line_feed_count;
        token_whitespace.space_count = space.initial_whitespace.space_count;
        delexer(writer, (Token *)&token_whitespace);
    }
    token_comment.self.self.kind = Token_comment;
    for(s64 index = 0; index < space.count; ++index)
    {
        {
            token_comment.type = space.comments[index].type;
            token_comment.self.text = space.comments[index].text;
            delexer(writer, (Token *)&token_comment);
        }
        {
            token_whitespace.line_feed_count = space.comments[index].whitespace.line_feed_count;
            token_whitespace.space_count = space.comments[index].whitespace.space_count;
            delexer(writer, (Token *)&token_whitespace);
        }
    }
}