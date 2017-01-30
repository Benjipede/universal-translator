typedef struct
{
    s64 newline_count, space_count;
} Whitespace;

typedef struct
{
    CommentType type;
    string text;
    Whitespace whitespace;
} Comment;

typedef struct
{
    Whitespace initial_whitespace;
    Comment *comments;
    s64 count;
} Space;

typedef enum
{
    Global_none,
    Global_unknown,
    Global_space,
    Global_Declaration,
} GlobalType;

typedef struct Global
{
    union
    {
        Space space;
        struct
        {
            struct Global *globals;
            s64 global_count;
        } array;
    };
    GlobalType type;
} Global;