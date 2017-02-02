#include "lib.h"
#include "reader.h"
#include "writer.h"

int main(int argc, char **argv)
{
    Pool pool = make_default_pool();
    FILE *langfile, *toolfile;
    
    char *initial_path = "w:/universal-translator/c/src/languages/";
    if(argc == 1)
    {
        printf("General case not yet supported. Use: support language [ tool... ]");
        return 0;
    }
    
    {
        char *argument = argv[1];
        string path;
        path.data = get_memory_align(&pool, strlen(initial_path) + strlen(argument) + strlen("/deparser.h") + 1, 1);
        path = copy_string_from_cstring(path.data, initial_path);
        path.count += copy_string(path.data + path.count, to_lower(string_from_cstring(argument))).count + 1;
        path.data[path.count-1] = '/';
        
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