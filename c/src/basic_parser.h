/*

unknown:
    token_unknown(str) -> multicomment(str)
    
space:
    whitespace comment space -> space_array(x1, *x2)
    whitespace -> space_array(x1, null)
    
whitespace:
    token_whitespace(no_newlines, no_spaces) -> whitespace(no_newlines, no_spaces)
    
comment:
    token_singlecomment(str) -> singlecomment(str)
    token_multicomment(str) -> multicomment(str)
    
type:
    token_variable(name) -> type(name)
    
globals:
    space global globals -> global_array(x1, x2)
    space
    empty
    
global:
    type space token_variable(name) space ; -> declaration(x1, x2, name)
    
*/

#include <stdlib.h>

Space parse_space(Lexer lexer, Reader *reader, string *storage, Stack *stack, Queue *que)
{
    Token token;
    Space result;
    result.count = 0;
    
    token = pop(stack);
    while(token.type == Token_whitespace || token.type == Token_comment)
    {
        if(token.type == Token_comment)  ++result.count;
        queue(que, token); // @Robustness: Handle adjacent whitespace tokens?
        token = lexer(reader, storage);
    }
    push(stack, token);
    
    s64 storage_used = sizeof(Comment) * result.count;
    ASSERT(storage_used <= storage->count)
    result.comments = (Comment *)storage->data;
    storage->count  -= storage_used;
    storage->data   += storage_used;
    
    token = dequeue(que);
    if(token.type == Token_whitespace)
    {
        result.initial_whitespace.newline_count  = token.newline_count;
        result.initial_whitespace.space_count    = token.space_count;
        token = dequeue(que);
    }
    else
    {
        result.initial_whitespace.newline_count  = 0;
        result.initial_whitespace.space_count    = 0;
    }
    
    for(s64 index = 0; token.type == Token_comment; ++index)
    {
        result.comments[index].type = token.comment.type;
        result.comments[index].text = token.comment.text;
        token = dequeue(que);
        if(token.type == Token_whitespace)
        {
            result.comments[index].whitespace.newline_count = token.newline_count;
            result.comments[index].whitespace.space_count   = token.space_count;
            token = dequeue(que);
        }
        else
        {
            result.comments[index].whitespace.newline_count = 0;
            result.comments[index].whitespace.space_count   = 0;
        }
    }
    
    return result;
}

#define GLOBALS_BUFFER_SIZE 0x10
Global globals_buffer[GLOBALS_BUFFER_SIZE];

Global parse_globals(Parser parser, Lexer lexer, Reader *reader, string *storage, Stack *stack, Queue *que)
{
    Global ast;
    ast.type = Global_globals;
    ast.globals.elements = globals_buffer;
    ast.globals.count = 0;
    for(Global global = parser(lexer, reader, storage, stack, que); global.type != Global_eof; global = parser(lexer, reader, storage, stack, que))
    {
        ast.globals.elements[ast.globals.count++] = global;
    }
    return ast;
}