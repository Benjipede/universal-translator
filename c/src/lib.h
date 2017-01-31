#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef float f32;
typedef double f64;

typedef s8 b8;
typedef s16 b16;
typedef s32 b32;
typedef s64 b64;

typedef uintptr_t umm;
typedef intptr_t smm;


typedef struct
{
    s64 count;
    u8 *data;
} string;

#include <string.h>
string string_from_c_string(char *c)
{
    string result;
    result.data = (u8 *)c;
    result.count = strlen(c);
    return result;
}

b8 is_string_equal_to_c_string(string text, char *c)
{
    for(s64 index = 0; index < text.count; ++index, ++c)
    {
        if(text.data[index] != *c)
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
    storage->count  -= basename.count;
    for(; *extension || *(extension - 1); ++extension)
    {
        *storage->data = (u8)*extension;
        ++storage->data;
        --storage->count;
    }
    return result;
}

#include <sys/stat.h>

#if 1
#define ASSERT(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define ASSERT(Expression)
#endif