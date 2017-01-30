#include <stdlib.h>
#include <stdio.h>
// TODO(casey): Complete assertion macro - don't worry everyone!
#if 1
#define ASSERT(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define ASSERT(Expression)
#endif

#include "lib.h"

#include "reader.h"
#include "tokens.h"
#include "lexer.h"
#include "syntax.h"
#include "parser.h"

#include "writer.h"
#include "delexer.h"
#include "deparser.h"

char buffer[0x10];

char *to_c_string(string s)
{
    for(s64 index = 0; index < s.count; ++index)
    {
        buffer[index] = s.data[index];
    }
    buffer[s.count] = 0;
    return buffer;
}

int main(int argc, char **argv)
{
    Reader reader;
    Lexer lexer;
    Parser parser;
    
    Writer writer;
    Delexer delexer;
    Deparser deparser;
    
    Stack stack;
    Queue que;
#define STACK_CAPACITY 0x100
#define QUEUE_CAPACITY 0x100
    Token stack_buffer[STACK_CAPACITY];
    Token queue_buffer[QUEUE_CAPACITY];
    
    adData data;
    
    if(argc < 3)
    {
        printf("Error expected two arguments.\n");
        return 0;
    }
    char *infilename = argv[1];
    char *outfilename = argv[2];
    reader = make_ascii_dumper(infilename, &data);
    writer = make_ascii_putter(outfilename);
    
    lexer = lex_simple;
    parser = parse_simple;
    
    delexer = delex_simple;
    deparser = deparse_simple;
    
    stack.elements = stack_buffer;
    stack.capacity = STACK_CAPACITY;
    que.elements = queue_buffer;
    que.capacity = QUEUE_CAPACITY;
    
    Global ast = parser(lexer, &reader, &stack, &que);
    deparser(delexer, &writer, ast);
    
    return 0;
}