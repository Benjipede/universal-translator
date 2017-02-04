#include "../lib/files_and_directories.h"
#include "streamers/streamer.h"

#include <stdio.h>
#include <sys/stat.h>

b8 directory_exists(char *path)
{
    struct stat sb;
    b8 result = stat(path, &sb) == 0;
    return result;
}

typedef enum
{
    Mode_none = 0x0,
    Mode_all = 0x1,
    Mode_new = 0x2,
    Mode_force = 0x4,
} Mode;

Mode handle_commandline_arguments(int argc, char **argv, Pool *pool, string *short_name, string *long_name, PointerArray *extension_array)
{
    Mode mode = Mode_none;
    
    char *argument;
    b8 force = 0;
    
    if(argc == 1)
        return Mode_all;
    
    short_name->count = 0;
    long_name->count = 0;
    *extension_array = make_pointer_array(8, pool);
    
    for(s64 index = 1; index < argc; ++index)
    {
        argument = argv[index];
        if(argument[0] == '-')
        {
            ++argument;
            if(argument[0] == 'a')
            {
                mode |= Mode_all;
            }
            else if(!strcmp(argument, "new"))
            {
                mode |= Mode_new;
                if(index+1 >= argc || argv[index+1][0] == '-')
                    continue;
                ++index;
                if(!short_name->count)
                {
                    *short_name = to_lower(string_from_cstring(argv[index]));
                    if(index+1 >= argc || argv[index+1][0] == '-')
                        continue;
                    ++index;
                }
                if(!long_name->count)
                {
                    *long_name = string_from_cstring(argv[index]);
                    if(index+1 >= argc || argv[index+1][0] == '-')
                        continue;
                    ++index;
                }
                if(!extension_array->count)
                {
                    for(; index < argc; ++index)
                    {
                        add_pointer(extension_array, (u8 *)argv[index]);
                        if(index+1 >= argc || argv[index+1][0] == '-')
                            break;
                    }
                }
            }
            else if(!strcmp(argument, "force"))
            {
                mode |= Mode_force;
            }
        }
        else
        {
            *short_name = to_lower(string_from_cstring(argument));
            ++index;
        }
    }
    
    return mode;
}

int main(int argc, char **argv)
{
    Pool pool = make_default_pool();
    
    string initial_path = string_from_cstring("w:/universal-translator/c/src/languages/");
    string path; // expected to be zero terminated
    string short_name; // expected to be zero terminated
    string long_name;
    PointerArray extension_array;
    
    Mode mode = handle_commandline_arguments(argc, argv, &pool, &short_name, &long_name, &extension_array);
    
    if(!long_name.count)
        long_name = short_name;
    
    if(mode & Mode_all)
    {
        printf("Mode -a is not yet supported\n");
        return 0;
    }
    
    if(!short_name.count)
    {
        printf("No language name was given\n");
        return 0;
    }
    
    for(s64 index = 0; index < short_name.count; ++index)
    {
        if(short_name.data[index] == '/')
        {
            printf("Short name contains slashes\n");
            return 0;
        }
    }
    
    path.count = initial_path.count + short_name.count + 1;
    #define MAX_FILENAME_COUNT 10 // @Hardcoded: SUPPORT.md deparser.h
    path.data = (u8 *)get_memory_align(&pool, path.count + MAX_FILENAME_COUNT + 1, 1);
    copy_string(path.data, initial_path);
    copy_string(path.data + initial_path.count, short_name);
    path.data[path.count-1] = '/';
    path.data[path.count] = 0;
    
    if(mode & Mode_new)
    {
        FILE *file;
        string filename;
        if(directory_exists((char *)path.data))
        {
            if(!(mode & Mode_force))
            {
                printf("languages/%s already exists. Are you sure that you want to erase the current code and start from scratch. If so use -force.\n", short_name.data);
                return 0;
            }
            printf("Forcing remake of %s\n", short_name.data);
        }
        else
        {
            if(!make_directory((char *)path.data))
            {
                printf("failed to create directory languages/%s\n", short_name.data);
                return 1;
            }
            printf("Directory languages/%s created\n", short_name.data);
        }
        
        {
            filename = path;
            filename.count += copy_string_from_cstring(filename.data + filename.count, "LANG.md").count;
            filename.data[filename.count] = 0;
            file = fopen((char *)filename.data, "wb");
            if(file)
            {
                fprintf(file,   "# %s\n\n"
                                "## Description\n\n"
                                "Short description of %s\n\n"
                                "## Tokens\n\n"
                                "whitespace,line comment,multiline comment\n\n"
                                "## Code Nodes\n\n"
                                "space:whitespace,line comment,multiline comment",
                                long_name.data, long_name.data);
                fclose(file);
                printf("LANG.md created\n");
            }
            else
            {
                perror((char *)filename.data);
            }
        }
        
        {
            filename = path;
            filename.count += copy_string_from_cstring(filename.data + filename.count, "SUPPORT.md").count;
            filename.data[filename.count] = 0;
            file = fopen((char *)filename.data, "wb");
            if(file)
            {
                fprintf(file,   "# Support for %s\n\n"
                                "- [ ] space\n\n"
                                "## Lexers\n\n"
                                "lex_%s\n\n"
                                "- [ ] whitespace\n"
                                "- [ ] line comment\n"
                                "- [ ] multiline comment\n\n"
                                "## Parsers\n\n"
                                "parse_%s\n\n"
                                "- [ ] space\n\n"
                                "## Deparsers\n\n"
                                "deparse_%s\n\n"
                                "- [ ] space\n\n"
                                "## Delexers\n\n"
                                "delex_%s\n\n"
                                "- [ ] whitespace\n"
                                "- [ ] line comment\n"
                                "- [ ] multiline comment\n\n",
                                long_name.data, short_name.data,
                                short_name.data, short_name.data, short_name.data);
                fclose(file);
                printf("SUPPORT.md created\n");
            }
            else
            {
                perror((char *)filename.data);
            }
        }
        
        {
            char *text =    "SUPPORT()\n"
                            "{\n"
                            "    \n"
                            "}";
            filename = path;
            filename.count += copy_string_from_cstring(filename.data + filename.count, "lexer.h").count;
            filename.data[filename.count] = 0;
            file = fopen((char *)filename.data, "wb");
            if(file)
            {
                fprintf(file, "Token lex_%s(Reader *reader, Pool *pool)\n%s", short_name.data, text);
                fclose(file);
                printf("lexer.h created\n");
            }
            else
            {
                perror((char *)filename.data);
            }
            
            filename = path;
            filename.count += copy_string_from_cstring(filename.data + filename.count, "parser.h").count;
            filename.data[filename.count] = 0;
            file = fopen((char *)filename.data, "wb");
            if(file)
            {
                fprintf(file, "Global parse_%s(Lexer lexer, Reader *reader, Pool *pool, Stack *stack, Queue *que)\n%s", short_name.data, text);
                printf("parser.h created\n");
                fclose(file);
            }
            else
            {
                perror((char *)filename.data);
            }
            
            filename = path;
            filename.count += copy_string_from_cstring(filename.data + filename.count, "deparser.h").count;
            filename.data[filename.count] = 0;
            file = fopen((char *)filename.data, "wb");
                printf("deparser.h created\n");
            if(file)
            {
                fprintf(file, "deparse_%s(Delexer delexer, Writer *writer, Global node)\n%s", short_name.data, text);
                fclose(file);
            }
            else
            {
                perror((char *)filename.data);
            }
            
            filename = path;
            filename.count += copy_string_from_cstring(filename.data + filename.count, "delexer.h").count;
            filename.data[filename.count] = 0;
            file = fopen((char *)filename.data, "wb");
            if(file)
            {
                fprintf(file, "delex_%s(Writer *writer, Token token)\n%s", short_name.data, text);
                fclose(file);
                printf("delexer.h created\n");
            }
            else
            {
                perror((char *)filename.data);
            }
        }
        
        return 0;
    }
    
    for(s64 index = 0; index < argc; ++index)
    {
        FILE *langfile, /**suppfile,*/ *toolfile;
        
        string langname = path;
        langname.count += copy_string_from_cstring(path.data + path.count, "LANG.md").count;
        langname.data[langname.count] = 0;
        langfile = fopen((char *)langname.data, "rb");
        if(!langfile)
        {
            perror((char *)langname.data);
            return 1;
        }
        
        string toolname = path;
        toolname.count += copy_string_from_cstring(path.data + path.count, "lexer.h").count;
        toolname.data[toolname.count] = 0;
        toolfile = fopen((char *)toolname.data, "rb");
        if(!toolfile)
        {
            perror((char *)toolname.data);
            return 1;
        }
        
        {
            string string_to_match = string_from_cstring("SUPPORT(");
            for(s64 match = 0; match < string_to_match.count; )
            {
                int c = fgetc(toolfile);
                if(c < 0)
                {
                    printf("No support information in %s\n", toolname.data);
                    return 0;
                }
                if(c == string_to_match.data[match])
                    ++match;
            }
            printf("FOUND!\n");
            /*
            while(1)
            {
                int c;
                switch(c)
                {
                    case ')':
                    {
                        
                    } break;
                    case ',':
                    {
                        
                    } break;
                    case '\\':
                    {
                        c = fgetc(
                    } break;
                    default:
                    {
                        if(c < 0)
                        {
                            printf("Syntax error");
                            return 0;
                        }
                    }
                }
            }
            */
        }
    }
    return 0;
}