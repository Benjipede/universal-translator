#ifndef UNICODE_H
#define UNICODE_H

#define eof 0xffffffff // Use as end of file character
#define sof 0xfffffffe // Use as start of file character

// Should
#define ONE_BYTE(arg)       (((u8)(arg) >> 7) ^ 0x1)
#define TWO_BYTES(arg)      (((u8)(arg) >> 5) ^ 0x1)
#define THREE_BYTES(arg)    (((u8)(arg) >> 4) ^ 0x1)
#define FOUR_BYTES(arg)     (((u8)(arg) >> 3) ^ 0x1)
#define MIDDLE_BYTE(arg)    (((u8)(arg) >> 6) ^ 0x1)
u8 utf8_size(u8 *c)
{
    if(ONE_BYTE(*c))
        return 1;
    if(TWO_BYTES(*c))
        return 2;
    if(THREE_BYTES(*c))
        return 3;
    if(FOUR_BYTES(*c))
        return 4;
    return 0;
}

void utf8_inc(u8 *c)
{
    for(++c; MIDDLE_BYTE(*c); ++c);
}
void utf8_dec(u8 *c)
{
    for(--c; MIDDLE_BYTE(*c); --c);
}

b8 is_whitespace(u32 c)
{
    b8 result = c == ' ' || c == '\n' || c == '\t';
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

#endif