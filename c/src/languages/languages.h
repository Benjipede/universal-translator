typedef struct
{
    char *name;
    char **extensions;
    s64 extension_count;
    
    Lexer lexer;
    Parser parser;
    Deparser deparser;
    Delexer delexer;
} Language;

#define SUPPORT(...)
#include "c/lang.h"
#include "simple/lang.h"

typedef Language (*GetLanguage)();
const GetLanguage get_language_array[] = {get_language_c, get_language_simple};

s64 find_language_by_name(char *name)
{
    for(s64 index = 0; index < array_count(get_language_array); ++index)
    {
        if(!strcmp(name, get_language_array[index]().name))
            return index;
    }
    return -1;
}

s64 find_language_by_extension(string extension)
{
    for(s64 j = 0; j < array_count(get_language_array); ++j)
    {
        for(s64 i = 0; i < get_language_array[j]().extension_count; ++i)
        {
            if(is_string_equal_to_cstring(extension, get_language_array[j]().extensions[i]))
                return j;
        }
    }
    return -1;
}