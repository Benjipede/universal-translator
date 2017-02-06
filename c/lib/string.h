#ifndef OUR_STRING_H
#define OUR_STRING_H

#include "pool.h"
#include "unicode.h"

#include <string.h>

typedef struct
{
    s64 count;
    u8 *data;
} string;

void append_character(string *text, u32 c)
{
    u8 *d = (u8 *)&c;
    u8 size = utf8_size(d);
    for(s64 index = 0; index < size; ++index)
        text->data[text->count++] = d[index];
}

string to_upper(string text)
{
    for(s64 index = 0; index < text.count; ++index)
    {
        text.data[index] = (u8)to_uppercase(text.data[index]);
    }
    return text;
}

string to_lower(string text)
{
    for(s64 index = 0; index < text.count; ++index)
    {
        text.data[index] = (u8)to_lowercase(text.data[index]);
    }
    return text;
}



string string_from_cstring(char *c)
{
    string result;
    result.data = (u8 *)c;
    result.count = strlen(c);
    return result;
}

string copy_string_from_cstring(u8 *ptr, char *c)
{
    string result;
    result.data = ptr;
    result.count = 0;
    while(*c)
    {
        result.data[result.count++] = *(c++);
    }
    return result;
}

string copy_string(u8 *ptr, string text)
{
    string result;
    result.data = ptr;
    for(s64 index = 0; index < text.count; ++index)
    {
        result.data[index] = text.data[index];
    }
    result.count = text.count;
    return result;
}

b8 are_strings_equal(string text1, string text2)
{
    if(text1.count != text2.count)
        return 0;
    for(s64 index = 0; index < text1.count; ++index)
    {
        if(text1.data[index] != text2.data[index])
            return 0;
    }
    return 1;
}

b8 is_string_equal_to_cstring(string text, char *c)
{
    for(s64 index = 0; index < text.count; ++index)
    {
        if(text.data[index] != c[index])
            return 0;
    }
    return 1;
}

b8 begins_with(char *c, char *d)
{
    for(s64 index = 0; d[index]; ++index)
    {
        if(c[index] != d[index])
            return 0;
    }
    return 1;
}

string get_filename_basename(char *argument)
{
    string result;
    result.data = (u8 *)argument;
    for(; *argument && *argument != '.'; ++argument);
    result.count = (u8 *)argument - result.data;
    return result;
}

string get_filename_extension(char *argument)
{
    string result;
    for(result.data = (u8 *)argument; *result.data; ++result.data)
    {
        if(*result.data == '.')
        {
            ++result.data;
            break;
        }
    }
    result.count = strlen((char *)result.data);
    return result;
}

char *make_filename(string basename, string extension, Pool *pool)
{
    char *result = (char *)get_memory_align(pool, basename.count + 1 + extension.count + 1, 1);
    copy_string((u8 *)result, basename);
    result[basename.count] = '.';
    copy_string((u8 *)result + basename.count + 1, extension);
    result[basename.count + 1 + extension.count] = 0;
    
    return result;
}

#endif