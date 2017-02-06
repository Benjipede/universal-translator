#ifndef FILES_AND_DIRECTORIES_H
#define FILES_AND_DIRECTORIES_H

#include "string.h"
#include "byte_array.h"

#include <stdio.h>
#include <sys/stat.h>
#include <windows.h>

b8 directory_exists(char *path)
{
    struct stat sb;
    b8 result = stat(path, &sb) == 0;
    return result;
}

b8 make_directory(char *path)
{
    if(CreateDirectory(path, NULL))
        return 1;
    return 0;
}

string read_identifier(FILE *file, Pool *pool)
{
    string result;
    ByteArray character_array;
    
    int c = fgetc(file);
    while(c == ' ' || c == '\n')
    {
        c = fgetc(file);
    }
    
    if(!is_alpha(c) && c != '_')
    {
        result.count = 0;
        fseek(file, -1, SEEK_CUR);
        return result;
    }
    
    character_array = make_byte_array(10, pool);
    add_byte(&character_array, (u8)c);
    
    for(c = fgetc(file); is_alphanumeric(c) || c == '_'; c = fgetc(file))
    {
        add_byte(&character_array, (u8)c);
    }
    
    result.data = character_array.data;
    result.count = character_array.count;
    return result;
}

b8 read_past_string(FILE *file, string text)
{
    for(s64 match = 0; match < text.count; )
    {
        int c = fgetc(file);
        if(c == EOF)
            return 0;
        if(c == text.data[match])
            ++match;
        else
            match = 0;
    }
    return 1;
}

b8 read_past_cstring(FILE *file, char *text)
{
    return read_past_string(file, string_from_cstring(text));
}


b8 read_past_string_break_at(FILE *file, string text, string break_at)
{
    s64 break_match = 0;
    for(s64 match = 0; match < text.count; )
    {
        int c = fgetc(file);
        if(c == EOF)
            return 0;
        if(c == break_at.data[break_match])
        {
            if(++break_match == break_at.count)
            {
                fseek(file, 1-(long int)break_at.count, SEEK_CUR);
                return 0;
            }
        }
        else
            break_match = 0;
        if(c == text.data[match])
            ++match;
        else
            match = 0;
    }
    return 1;
}

b8 read_past_cstring_break_at(FILE *file, char *text, char *break_at)
{
    return read_past_string_break_at(file, string_from_cstring(text), string_from_cstring(break_at));
}

#endif