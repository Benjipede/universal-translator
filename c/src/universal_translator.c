#include "lib.h"

#include "reader.h"
#include "tokens.h"
#include "lexer.h"
#include "syntax.h"
#include "parser.h"

#include "writer.h"
#include "delexer.h"
#include "deparser.h"

#include "languages/languages.h"

#define STORAGE_SIZE 0x400*0x400 // 1 MB
u8 storage_buffer[STORAGE_SIZE];

#define STACK_CAPACITY 0x100
#define QUEUE_CAPACITY 0x100

Token stack_buffer[STACK_CAPACITY];
Token queue_buffer[QUEUE_CAPACITY];

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
    
    string storage;
    
    if(argc < 3)
    {
        printf("Error expected two arguments.\n");
        return 0;
    }
    storage.data = storage_buffer;
    storage.count = STORAGE_SIZE;
    
    stack = make_stack(stack_buffer, STACK_CAPACITY);
    que = make_queue(queue_buffer, QUEUE_CAPACITY);
    
    char *infilename = argv[1];
    char *outfilename = argv[2];
    reader = make_ascii_dumper(infilename, &storage);
    writer = make_ascii_putter(outfilename);
    
    lexer = lex_simple;
    parser = parse_simple;
    
    delexer = delex_simple;
    deparser = deparse_simple;
    
    {
        Global ast = parse_globals(parser, lexer, &reader, &storage, &stack, &que);
        deparser(delexer, &writer, ast);
    }
    
    return 0;
}