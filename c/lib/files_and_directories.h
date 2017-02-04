#ifndef FILES_AND_DIRECTORIES_H
#define FILES_AND_DIRECTORIES_H

#include "string.h"

#include <windows.h>

b8 make_directory(char *path)
{
    if(CreateDirectory(path, NULL))
        return 1;
    return 0;
}

#endif