#ifndef UTF8_H
#define UTF8_H

#include "nicetypes.h"

//
// Characters
//

u32 char_to_lower(u32 c)
{
    u32 result;
    if('A' <= c && c <= 'Z')
        result = c + ('a' - 'A');
    else
        result = c;
    return result;
}

b8 is_uppercase(u32 c)
{
    b8 result = 'A' <= c && c <= 'Z';
    return result;
}

b8 is_lowercase(u32 c)
{
    b8 result = 'a' <= c && c <= 'z';
    return result;
}

b8 is_alpha(u32 c)
{
    b8 result = is_uppercase(c) || is_lowercase(c);
    return result;
}

u32 to_uppercase(u32 c)
{
    u32 result;
    if(is_lowercase(c))
        result = c - ('a' - 'A');
    else
        result = c;
    return result;
}

u32 to_lowercase(u32 c)
{
    u32 result;
    if(is_uppercase(c))
        result = c + ('a' - 'A');
    else
        result = c;
    return result;
}

b8 is_digit(u32 c)
{
    b8 result = ('0' <= c && c <= '9');
    return result;
}

b8 is_alphanumeric(u32 c)
{
    b8 result = is_alpha(c) || is_digit(c);
    return result;
}

//
// Strings
//

typedef struct
{
    s64 count;
    u8 *data;
} string;

void to_upper(string text)
{
    for(s64 index = 0; index < text.count; ++index)
    {
        text.data[index] = (u8)to_uppercase(text.data[index]);
    }
}

void to_lower(string text)
{
    for(s64 index = 0; index < text.count; ++index)
    {
        text.data[index] = (u8)to_lowercase(text.data[index]);
    }
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

b8 is_string_equal_to_cstring(string text, char *c)
{
    for(s64 index = 0; index < text.count; ++index)
    {
        if(text.data[index] != c[index])
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

char *make_filename(string *storage, string basename, char *extension)
{
    char *result;
    result = (char *)storage->data;
    memcpy(storage->data, basename.data, basename.count);
    storage->data   += basename.count;
    storage->count  -= basename.count+1;
    *storage->data = (u8)'.';
    ++storage->data;
    
    for(; *extension; ++extension)
    {
        *storage->data = (u8)*extension;
        ++storage->data;
        --storage->count;
    }
    *storage->data = 0;
    ++storage->data;
    --storage->count;
    
    return result;
}

#endif