#include "../lib/utf8.h"
#include "../lib/pool.h"

#define array_count(Array) (sizeof(Array) / sizeof((Array)[0]))

#if 1
#define ASSERT(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define ASSERT(Expression)
#endif