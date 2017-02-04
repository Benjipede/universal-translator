#ifndef FILES_AND_DIRECTORIES_H
#define FILES_AND_DIRECTORIES_H

#include "string.h"

#include <stdio.h>
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

b8 move_past_string(FILE *file, string text)
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

#endif