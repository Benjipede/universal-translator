#include <stdlib.h>

Token *get_token(Lexer lexer, Reader *reader, Pool *pool, Queue *bank)
{
    if(bank->array.count == 0)
        return lexer(reader, pool);
    return (Token *)dequeue(bank);
}

Space parse_space(Lexer lexer, Reader *reader, Pool *pool, Queue *bank, Queue *que)
{
    Token *token;
    Space result;
    result.count = 0;
    
    token = get_token(lexer, reader, pool, bank);
    while(token->kind == Token_whitespace || token->kind == Token_comment)
    {
        if(token->kind == Token_comment)  ++result.count;
        queue(que, (u8 *)token); // @Robustness: Handle adjacent whitespace tokens?
        token = lexer(reader, pool);
    }
    queue(bank, (u8 *)token);
    
    result.comments = (Comment *)get_memory(pool, sizeof(Comment) * result.count);
    
    token = (Token *)dequeue(que);
    if(token && token->kind == Token_whitespace)
    {
        result.initial_whitespace.line_feed_count  = ((TokenWhitespace *)token)->line_feed_count;
        result.initial_whitespace.space_count      = ((TokenWhitespace *)token)->space_count;
        token = (Token *)dequeue(que);
    }
    else
    {
        result.initial_whitespace.line_feed_count  = 0;
        result.initial_whitespace.space_count    = 0;
    }
    
    for(s64 index = 0; token && token->kind == Token_comment; ++index)
    {
        result.comments[index].type = ((TokenComment *)token)->type;
        result.comments[index].text = ((TokenText *)token)->text;
        token = (Token *)dequeue(que);
        if(token && token->kind == Token_whitespace)
        {
            result.comments[index].whitespace.line_feed_count = ((TokenWhitespace *)token)->line_feed_count;
            result.comments[index].whitespace.space_count   = ((TokenWhitespace *)token)->space_count;
            token = (Token *)dequeue(que);
        }
        else
        {
            result.comments[index].whitespace.line_feed_count = 0;
            result.comments[index].whitespace.space_count   = 0;
        }
    }
    
    return result;
}

//
//
//
Global parse_globals(Parser parser, Lexer lexer, Reader *reader, Pool *pool)
{
#define BANK_CAPACITY 256
#define QUEUE_CAPACITY 256
    Queue bank = make_queue(BANK_CAPACITY, pool);
    Queue que = make_queue(QUEUE_CAPACITY, pool);
    
    Global ast;
    ast.type = Global_globals;
    ast.globals.elements = (Global *)get_memory(pool, 0);
    ast.globals.count = 0;
    
    for(Global global = parser(lexer, reader, pool, &bank, &que); global.type != Global_eof; global = parser(lexer, reader, pool, &bank, &que))
    {
        Global *memory = (Global *)get_more_memory(pool, ast.globals.elements, sizeof(Global) * (ast.globals.count+1), sizeof(Global) * ast.globals.count);
        ASSERT(memory)
        ast.globals.elements = memory;
        ast.globals.elements[ast.globals.count++] = global;
    }
    
    return ast;
}