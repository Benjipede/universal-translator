#include "../lib/files_and_directories.h"
#include "../lib/array_array.h"
#include "../lib/byte_array.h"
#include "../lib/string_array.h"

#define MAX_FILENAME_COUNT 10 // @Hardcoded: SUPPORT.md deparser.h



typedef enum
{
    Mode_none = 0x0,
    Mode_all = 0x1,
    Mode_new = 0x2,
    Mode_force = 0x4,
    Mode_verbose = 0x8,
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
            if(argument[0] == 'a' || !strcmp(argument, "all"))
            {
                mode |= Mode_all;
            }
            if(argument[0] == 'v' || !strcmp(argument, "verbose"))
            {
                mode |= Mode_verbose;
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
        }
    }
    
    return mode;
}



typedef struct
{
    string name;
    StringArray dependency_array;
} CodeNodeDeclaration;

// Set pool to NULL to use malloc as allocator
typedef struct
{
    CodeNodeDeclaration *data;
    s64 count;
    s64 capacity;
    
    struct Pool *pool;
} CodeNodeDeclarationArray;

CodeNodeDeclarationArray make_code_node_array(s64 capacity, struct Pool *pool)
{
    CodeNodeDeclarationArray array;
    array.count = 0;
    array.capacity = 0;
    array.pool = pool;
    
    CodeNodeDeclaration *memory;
    if(pool)
        memory = (CodeNodeDeclaration *)get_memory(pool, sizeof(CodeNodeDeclaration) * capacity);
    else
        memory = (CodeNodeDeclaration *)malloc(sizeof(CodeNodeDeclaration) * capacity);
    ASSERT(memory);
    array.data = memory;
    array.capacity = capacity;
    
    return array;
}

CodeNodeDeclarationArray make_default_code_node_array()
{
    return make_code_node_array(8, NULL);
}

void reserve_code_nodes(CodeNodeDeclarationArray *array, s64 amount)
{
    if(array->capacity < amount)
    {
        CodeNodeDeclaration *memory;
        if(array->pool)
            memory = (CodeNodeDeclaration *)get_more_memory(array->pool, (u8 *)array->data, sizeof(CodeNodeDeclaration) * amount, sizeof(CodeNodeDeclaration) * array->capacity);
        else
            memory = (CodeNodeDeclaration *)realloc(array->data, sizeof(CodeNodeDeclaration) * amount);
        ASSERT(memory);
        array->data = memory;
        array->capacity = amount;
    }
}

void add_code_node(CodeNodeDeclarationArray *array, CodeNodeDeclaration decl)
{
    if(array->count >= array->capacity)
    {
        reserve_code_nodes(array, 2*array->capacity);
    }
    
    array->data[array->count++] = decl;
}



#define STARTING_TOKEN_CAPACITY 20
#define STARTING_CODE_NODE_CAPACITY 20
#define STARTING_NAME_CAPACITY 20

StringArray read_support_list(FILE *file, Pool *pool, string short_name)
{
    StringArray token_array = make_string_array(STARTING_TOKEN_CAPACITY, pool);
    ByteArray character_array = make_byte_array(STARTING_NAME_CAPACITY, pool);
    string token_name;
    b8 after_line_feed = 0;
    b8 cont = 1;
    
    read_past_cstring(file, "(");
    
    while(cont)
    {
        int c = fgetc(file);
        switch(c)
        {
            case EOF:
            {
                printf("%s/LANG.md: Missing ')'\n", short_name.data);
                printf("%s/LANG.md does not contain header '## Code Nodes'\n", short_name.data);
                token_array.count = -1;
                cont = 0;
            } break;
            case '\n':
            {
                after_line_feed = 1;
            } continue;
            case ',':
            {
                if(character_array.count)
                {
                    token_name.data = character_array.data;
                    token_name.count = character_array.count;
                    add_string(&token_array, token_name);
                    character_array = make_byte_array(STARTING_NAME_CAPACITY, pool);
                }
            } break;
            case ')':
            {
                cont = 0;
                if(character_array.count)
                {
                    token_name.data = character_array.data;
                    token_name.count = character_array.count;
                    add_string(&token_array, token_name);
                }
            } break;
            case '#':
            {
                if(after_line_feed == 1)
                {
                    printf("%s/LANG.md: Missing ')'\n", short_name.data);
                    token_array.count = -1;
                    cont = 0;
                }
                else
                    add_byte(&character_array, (u8)c);
            } break;
            case '\\':
            {
                c = fgetc(file);
                if(c == ' ')
                {
                    for(c = fgetc(file); c == ' '; c = fgetc(file));
                    fseek(file, -1, SEEK_CUR);
                    break;
                }
                
                switch(c)
                {
                    case '\n' :
                    case '\r' :
                    case '\t' :
                    case '\v' :
                        c = fgetc(file); break;
                    case 'n': c = '\n'; break;
                    case 'r': c = '\r'; break;
                    case 't': c = '\t'; break;
                    case 'v': c = '\v'; break;
                }
                add_byte(&character_array, (u8)c);
            } break;
            case '\t':
            case '\r':
            {
                // Ignore
            } break;
            default:
            {
                add_byte(&character_array, (u8)c);
            }
        }
        after_line_feed = 0;
    }
    return token_array;
}


void read_tool_support_information(FILE *file, Pool *pool, StringArray *name_array, ArrayArray *support_array_array,  StringArray token_array, string short_name, Mode mode)
{
    for(s64 tool_index = 0; read_past_cstring(file,"Token "); ++tool_index)
    {
        StringArray supported_name_array;
        ByteArray support_array = make_byte_array(token_array.count, pool);
        string name = read_identifier(file, pool);
        s64 up_to = -1; // Used to improve best case runtime to linear. Removing from searced list would be even better.
        for(; support_array.count < token_array.count; ++support_array.count)
        {
            support_array.data[support_array.count] = 0;
        }
        
        add_string(name_array, name);
        add_array(support_array_array, *(PointerArray *)&support_array);
        
        if((mode & Mode_verbose) && !read_past_cstring_break_at(file, "SUPPORT", "Token "))
        {
            printf("\n%.*s does not appear to have any support information\n", (int)name.count, name.data);
            if(token_array.count)
                support_array.data[0] = 0xff; // Error code for this case
            continue;
        }
        
        supported_name_array = read_support_list(file, pool, short_name);
        //
        // Check for duplicates in supported_name_array
        // and report warning/error
        //
        
        for(s64 index = 0; index < supported_name_array.count; ++index)
        {
            s64 search_index = 0;
            for(; search_index < token_array.count; ++search_index)
            {
                if(are_strings_equal(supported_name_array.data[index], token_array.data[search_index]))
                {
                    up_to += (search_index == up_to + 1) ? 1 : 0;
                    support_array.data[search_index] = 1;
                    break;
                }
            }
            if(search_index == token_array.count)
            {
                printf("%.*s is said to be supported by lexer _, but it is not specified as a token in %.*s/LANG.md\n", (int)supported_name_array.data[index].count, supported_name_array.data[index].data, (int)short_name.count, short_name.data);
                continue;
            }
        }
    }
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
                                "(whitespace,line comment,multiline comment)\n\n"
                                "## Code Nodes\n\n"
                                "space:(whitespace,line comment,multiline comment)",
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
    
    
    if(!directory_exists((char *)path.data))
    {
        printf("languages/%s does not exist.\nRun support -new %s to make it and all the files needed for a language.\n", short_name.data, short_name.data);
        return 0;
    }
    

    {
        StringArray token_array;
        StringArray node_array;
        ArrayArray node_dependency_array_array;
        StringArray lexer_name_array/*, parser_name_array, deparser_name_array, delexer_name_array*/;
        ArrayArray lexer_array_array/*, parser_array_array, deparser_array_array, delexer_array_array*/;
        
        FILE *langfile, *suppfile, *toolfile;
        string langname, suppname, toolname;
        char *text;
        
        text = "LANG.md";
        langname.count = path.count + strlen(text);
        langname.data = (u8 *)get_memory_align(&pool, langname.count + 1, 1);
        copy_string(langname.data, path);
        copy_string_from_cstring(langname.data + path.count, text);
        langname.data[langname.count] = 0;
        langfile = fopen((char *)langname.data, "rb");
        if(!langfile)
        {
            perror((char *)langname.data);
            return 1;
        }
        
        {
            //
            // Tokens
            //
            
            if(!read_past_cstring(langfile, "## Tokens"))
            {
                printf("%s/LANG.md does not contain header '## Tokens'", short_name.data);
                return 0;
            }
            
            token_array = read_support_list(langfile, &pool, short_name);
            
            if(mode & Mode_verbose)
            {
                printf("\n(");
                for(s64 index = 0; index < token_array.count; ++index)
                {
                    for(s64 j = 0; j < token_array.data[index].count; ++j)
                        printf("%c", (char)token_array.data[index].data[j]);
                    putchar(',');
                }
                printf(")\n\n");
            }
            
            //
            // Code Nodes
            //
            
            if(!read_past_cstring(langfile, "## Code Nodes"))
            {
                printf("%s/LANG.md does not contain header '## Code Nodes'", short_name.data);
                return 0;
            }
            
            {
                ByteArray character_array = make_byte_array(STARTING_NAME_CAPACITY, &pool);
                b8 after_line_feed = 0;
                b8 cont = 1;
                
                node_array = make_string_array(STARTING_CODE_NODE_CAPACITY, &pool);
                node_dependency_array_array = make_array_array(STARTING_CODE_NODE_CAPACITY, &pool);
                
                
                while(cont)
                {
                    int c = fgetc(langfile);
                    switch(c)
                    {
                        case EOF:
                        {
                            cont = 0;
                            if(character_array.count)
                            {
                                printf("No ':' or '( dependency... )' after name in code node declaration\n");
                            }
                        } break;
                        case '\n':
                        {
                            after_line_feed = 1;
                        } continue;
                        case ':':
                        {
                            if(character_array.count)
                            {
                                string name;
                                StringArray dep_arr = read_support_list(langfile, &pool, short_name);
                                name.data = character_array.data;
                                name.count = character_array.count;
                                add_string(&node_array, name);
                                add_array(&node_dependency_array_array, *(PointerArray *)&dep_arr);
                                character_array = make_byte_array(STARTING_NAME_CAPACITY, &pool);
                            }
                        } break;
                        case '#':
                        {
                            if(after_line_feed == 1)
                            {
                                cont = 0;
                                if(character_array.count)
                                {
                                    printf("No ':' and '( dependency... )' after name in code node declaration\n");
                                }
                            }
                            else
                                add_byte(&character_array, (u8)c);
                        } break;
                        case '\\':
                        {
                            c = fgetc(langfile);
                            if(c == ' ')
                            {
                                for(c = fgetc(langfile); c == ' '; c = fgetc(langfile));
                                fseek(langfile, -1, SEEK_CUR);
                                break;
                            }
                            
                            switch(c)
                            {
                                case '\n' :
                                case '\r' :
                                case '\t' :
                                case '\v' :
                                    c = fgetc(langfile); break;
                                case 'n': c = '\n'; break;
                                case 'r': c = '\r'; break;
                                case 't': c = '\t'; break;
                                case 'v': c = '\v'; break;
                            }
                            add_byte(&character_array, (u8)c);
                        } break;
                        case '\t':
                        case '\r':
                        {
                            // Ignore
                        } break;
                        default:
                        {
                            add_byte(&character_array, (u8)c);
                        }
                    }
                    after_line_feed = 0;
                }
            }
        }
        fclose(langfile);
        printf("Done reading syntax information from %s/LANG.md\n", short_name.data);
        
        if(mode & Mode_verbose)
        {
            putchar('\n');
            for(s64 index = 0; index < node_array.count; ++index)
            {
                for(s64 j = 0; j < node_array.data[index].count; ++j)
                    printf("%c", (char)node_array.data[index].data[j]);
                putchar(':');
                putchar('(');
                
                StringArray *text_array = (StringArray *)&node_dependency_array_array.data[index];
                for(s64 i = 0; i < text_array->count; ++i)
                {
                    for(s64 j = 0; j < text_array->data[i].count; ++j)
                        printf("%c", (char)text_array->data[i].data[j]);
                    putchar(',');
                }
                putchar(')');
                putchar('\n');
            }
            putchar('\n');
        }
        
        
        toolname.data = (u8 *)get_memory_align(&pool, path.count + MAX_FILENAME_COUNT + 1, 1);
        copy_string(toolname.data, path);
        
        text = "lexer.h";
        toolname.count = path.count + strlen(text);
        copy_string_from_cstring(toolname.data + path.count, text);
        toolname.data[toolname.count] = 0;
        toolfile = fopen((char *)toolname.data, "rb");
        if(!toolfile)
        {
            perror((char *)toolname.data);
            return 1;
        }
        
        lexer_name_array = make_string_array(1, &pool);
        lexer_array_array = make_array_array(1, &pool);
        
        read_tool_support_information(toolfile, &pool, &lexer_name_array, &lexer_array_array, token_array, short_name, mode);
        
        text = "SUPPORT.md";
        suppname.count = path.count + strlen(text);
        suppname.data = (u8 *)get_memory_align(&pool, suppname.count + 1, 1);
        copy_string(suppname.data, path);
        copy_string_from_cstring(suppname.data + path.count, text);
        suppname.data[suppname.count] = 0;
        suppfile = fopen((char *)suppname.data, "rb");
        if(!suppfile)
        {
            perror((char *)suppname.data);
            return 1;
        }
        
    }
    return 0;
}