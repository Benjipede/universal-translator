typedef struct Writer
{
    void *data;
    
    void (*put)(struct Writer *, u32);
} Writer;

#include "writers/ascii_putter.h"