/*

  This is a command line interface - and for now the only interface

 */

#include "../bottleneck.h"

#include <stdio.h>

//
// @Hardcoded
//
#define STACK_CAPACITY 0x100
#define QUEUE_CAPACITY 0x100
Token stack_buffer[STACK_CAPACITY];
Token queue_buffer[QUEUE_CAPACITY];

b8 handle_commandline_arguments(int argc, char **argv, Pool *pool, char **source, char **destination, Reader *reader, Lexer *lexer, Parser *parser, Deparser *deparser, Delexer *delexer, Writer *writer)
{
    s64 source_language, target_language;
    b8 infer_extensions;
    
    if(argc == 1)
    {
        char *description =
        "usage: unitrl \"source filename\"\n"
        "              \"destination filename\" [ option... ]\n"
        "       unitrl \"source filename\"\n"
        "              -to \"target language\" [ option... ]\n"
        "       unitrl \"source basename\"\n"
        "              -from \"source language\" -to \"target language\" [ option... ]\n"
        "       unitrl -help\n"
        "\n"
        "options:\n"
        "   -from [language]    Set source languange\n"
        "   -to [language]      Set target languange\n"
        "   -no_ext             Disable extension-inference for filenames\n";
        printf(description);
        return 0;
    }
    if(!strcmp(argv[1], "-help"))
    {
        char *verbose_description =
        "usage: unitrl \"source filename\"\n"
        "              \"destination filename\" [ option... ]\n"
        "       unitrl \"source filename\"\n"
        "              -to \"target language\" [ option... ]\n"
        "       unitrl \"source basename\"\n"
        "              -from \"source language\" -to \"target language\" [ option... ]\n"
        "       unitrl -help\n"
        "\n"
        "options:\n"
        "   -from [language]    Set source languange. Otherwise source language is\n"
        "                       inferred from source file extension.\n"
        "   -to [language]      Set target languange.\n"
        "                       Otherwise source language is inferred from target file\n"
        "                       extension.\n"
        "   -no_ext             Disable extension-inference for filenames.\n";
        printf(verbose_description);
        return 0;
    }
    
    source_language = -1;
    target_language = -1;
    
    *source = NULL;
    *destination = NULL;
    
    infer_extensions = 1;
    
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
                {
                    printf("-from was given as last argument.\n");
                    return 0;
                }
                argument = argv[index];
                source_language = find_language_by_name(string_from_cstring(argument));
                if(source_language < 0)
                {
                    printf("%s is not a supported language.\n", argument);
                }
            }
            else if(!strcmp(argument, "to"))
            {
                ++index;
                if(index == argc)
                {
                    printf("-to was given as last argument.\n");
                    return 0;
                }
                argument = argv[index];
                target_language = find_language_by_name(string_from_cstring(argument));
                if(target_language < 0)
                {
                    printf("%s is not a supported language.\n", argument);
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
            if(source_language < 0)
            {
                source_language = find_language_by_extension(extension);
                if(source_language < 0)
                {
                    printf("Source language could not be inferred from source file extension '%s'.\n", (char *)extension.data); // null termination is assumed
                    return 0;
                }
            }
        }
        else if(source_language >= 0)
        {
            if(infer_extensions && get_language_array[source_language]().extension_count)
                *source = make_filename(get_filename_basename(*source), get_language_array[source_language]().extensions[0], pool);
        }
        else
        {
            printf("Source language was not set.\nUse the option '-from [language]' or pass the the source file name with extension to set the source languange.\n");
            return 0;
        }
    }
    
    {
        
        if(!*destination)
        {
            if(target_language >= 0)
            {
                if(infer_extensions && get_language_array[target_language]().extension_count)
                    *destination = make_filename(get_filename_basename(*source), get_language_array[target_language]().extensions[0], pool);
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
                if(target_language < 0)
                {
                    target_language = find_language_by_extension(extension);
                    if(source_language < 0)
                    {
                        printf("Target language could not be inferred from destination file extension '%s'.\n", (char *)extension.data); // null termination is assumed
                        return 0;
                    }
                }
            }
            else if(target_language >= 0)
            {
                if(infer_extensions && get_language_array[target_language]().extension_count)
                    *destination = make_filename(get_filename_basename(*destination), get_language_array[target_language]().extensions[0], pool);
            }
            else
            {
                printf("Target language was not set.\nTo set target language use the option '-to [language]' or pass the the destination filename with extension.\n");
                return 0;
            }
        }
    }
    
    *lexer = get_language_array[source_language]().lexer;
    *parser = get_language_array[source_language]().parser;
    
    *delexer = get_language_array[target_language]().delexer;
    *deparser = get_language_array[target_language]().deparser;
    
    *reader = make_ascii_dumper(*source, pool);
    if(!reader->data)
    {
        perror(*source);
        return 0;
    }
    *writer = make_ascii_putter(*destination);
    if(!writer->data)
    {
        perror(*destination);
        return 0;
    }
    
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
    
    Pool pool = make_default_pool();
    
    {
        char *source, *destination;
        if(!handle_commandline_arguments(argc, argv, &pool, &source, &destination, &reader, &lexer, &parser, &deparser, &delexer, &writer))
            return 0;
    }
    stack = make_stack(stack_buffer, STACK_CAPACITY);
    que = make_queue(queue_buffer, QUEUE_CAPACITY);
    
    {
        Global ast = parse_globals(parser, lexer, &reader, &pool, &stack, &que);
        deparser(delexer, &writer, ast);
    }
    
    return 0;
}