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

b8 handle_commandline_arguments(int argc, char **argv, string *storage, char **source, char **destination, Reader *reader, Lexer *lexer, Parser *parser, Deparser *deparser, Delexer *delexer, Writer *writer)
{   
    b8 infer_extensions;
    char *description;
    infer_extensions = 1;
    description = "usage:\nuniversal-translater source.c destination.sim [options]\nuniversal-translater source.c [options]\nuniversal-translater source [options]\n\noptions\n    -from [language]    Set source languange. Otherwise source language is inferred from source file extension.\n    -to [language]    Set target languange. Otherwise source language is inferred from target file extension.\n    -no_ext    Dont infer extensions for filenames given without extension.\n";
    if(argc == 1)
    {
        printf(description);
        return 0;
    }
    
    *lexer = NULL;
    *parser = NULL;
    
    *delexer = NULL;
    *deparser = NULL;
    
    *source = NULL;
    *destination = NULL;
    
    for(s64 index = 1; index < argc; ++index)
    {
        char *argument = argv[index];
        if(argument[0] == '-')
        {
            ++argument;
            if(!strcmp(argument, "from"))
            {
                ++index;
                if(index == argc)
                    return 0;
                argument = argv[index];
                if(!strcmp(argument, "c"))
                {
                    *lexer = lex_c;
                    *parser = parse_c;
                }
                else if(!strcmp(argument, "simple"))
                {
                    *lexer = lex_simple;
                    *parser = parse_simple;
                }
                else
                {
                    printf("Unknown language '%s'.\n", argument);
                    return 0;
                }
            }
            else if(!strcmp(argument, "to"))
            {
                ++index;
                if(index == argc)
                    return 0;
                argument = argv[index];
                if(!strcmp(argument, "c"))
                {
                    *delexer = delex_c;
                    *deparser = deparse_c;
                }
                else if(!strcmp(argument, "simple"))
                {
                    *delexer = delex_simple;
                    *deparser = deparse_simple;
                }
                else
                {
                    printf("Unknown language '%s'.\n", argument);
                    return 0;
                }
            }
            else if(!strcmp(argument, "no_ext"))
            {
                infer_extensions = 0;
            }
        }
        else
        {
            if(!*source)
            {
                *source = argument;
            }
            else
            {
                *destination = argument;
            }
        }
    }
    
    if(!*source)
    {
        printf("Source file was not given.\nSimply pass the name of a file as the first non-option argument to set the source file.\n");
        return 0;
    }
    
    {
        string extension = get_filename_extension(*source);
        if(extension.count)
        {
            if(!*lexer)
            {
                if(is_string_equal_to_c_string(extension, "c"))
                {
                    *lexer = lex_c;
                    *parser = parse_c;
                }
                else if(is_string_equal_to_c_string(extension, "sim"))
                {
                    *lexer = lex_simple;
                    *parser = parse_simple;
                }
            }
        }
        else if(*lexer == lex_c)
        {
            if(infer_extensions)  *source = make_filename(storage, get_filename_basename(*source), "c");
        }
        else if(*lexer == lex_simple)
        {
            if(infer_extensions)  *source = make_filename(storage, get_filename_basename(*source), "sim");
        }
        else
        {
            printf("Source language was not set.\nUse the option '-from [language]' or pass the the source file name with extension to set the source languange.\n");
            return 0;
        }
    }
    
    {
        FILE *file;
        file = fopen(*source, "rb");
        if(file)
            fclose(file);
        else
        {
            perror(*source);
            return 0;
        }
    }
    
    {
        
        if(!*destination)
        {
            if(*delexer == delex_c)
            {
                *destination = make_filename(storage, get_filename_basename(*source), "c");
            }
            else if(*delexer == delex_simple)
            {
                *destination = make_filename(storage, get_filename_basename(*source), "sim");
            }
            else
            {
                printf("Neither destination filename nor target language was not given.\nTo set the target language and use the source basename as destination basename pass '-to [language]'.\nTo set a different destination filename pass a filename as the second non-option argument. The target language will be set accordingly if the given filename has an extension. Otherwise pass '-to [language]' to set the target language.\n");
                return 0;
            }
        }
        else
        {
            string extension = get_filename_extension(*destination);
            if(extension.count)
            {
                if(!*delexer)
                {
                    if(is_string_equal_to_c_string(extension, "c"))
                    {
                        *delexer = delex_c;
                        *deparser = deparse_c;
                    }
                    else if(is_string_equal_to_c_string(extension, "sim"))
                    {
                        *delexer = delex_simple;
                        *deparser = deparse_simple;
                    }
                }
            }
            else if(*delexer == delex_c)
            {
                if(infer_extensions)  *destination = make_filename(storage, get_filename_basename(*destination), "c");
            }
            else if(*delexer == delex_simple)
            {
                if(infer_extensions)  *destination = make_filename(storage, get_filename_basename(*destination), "sim");
            }
            else
            {
                printf("Target language was not set.\nTo set target language use the option '-to [language]' or pass the the destination filename with extension.\n");
                return 0;
            }
        }
    }
        
    
    *reader = make_ascii_dumper(*source, storage);
    *writer = make_ascii_putter(*destination);
    
    return 1;
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
    
    string storage;
    storage.data = storage_buffer;
    storage.count = STORAGE_SIZE;
    
    {
        char *source, *destination;
        if(!handle_commandline_arguments(argc, argv, &storage, &source, &destination, &reader, &lexer, &parser, &deparser, &delexer, &writer))
            return 0;
    }
    stack = make_stack(stack_buffer, STACK_CAPACITY);
    que = make_queue(queue_buffer, QUEUE_CAPACITY);
    
    {
        Global ast = parse_globals(parser, lexer, &reader, &storage, &stack, &que);
        deparser(delexer, &writer, ast);
    }
    
    return 0;
}