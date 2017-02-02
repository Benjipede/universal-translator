#include "lib.h"

//#include <memory.h>

#define STORAGE_SIZE 0x400*0x400 // 1 MB
u8 storage_buffer[STORAGE_SIZE];

int main(int argc, char **argv)
{
    string storage;
    storage.data = storage_buffer;
    storage.count = STORAGE_SIZE;
    
    if(argc > 1)
    {
        string path = copy_string_from_cstring(storage.data, argv[1]);
        storage.data    += path.count;
        storage.count   -= path.count;
        to_lower(path);
        printf("path: %s\n", path.data);
        if(argc > 2)
        {
            
            return 0;
        }
        printf("All tool types are not yet supported. Use: support language [ tool... ]");
        return 0;
    }
    
    printf("General case not yet supported. Use: support language [ tool... ]");
    return 0;
}