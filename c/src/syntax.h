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
    Type_none,
    Type_unknown,
    Type_language_specific,
    Type_infer,
    Type_identifier,
    Type_multiple,
    Type_tuple,
    Type_pointer,
    Type_array,
    Type_procedure,
    Type_function,
} TypeType;

typedef struct Type
{
    union
    {
        string text;
    };
    TypeType type;
} Type;

typedef enum
{
    Expression_none,
    Expression_unknown,
    Expression_language_specific,
    Expression_identifier,
    Expression_procedure_lambda,
    Expression_procedure_call,
    Expression_function_lambda,
    Expression_function_evaluation,
    Expression_literal,
} ExpressionType;

typedef struct Literal
{
    union
    {
        string text;
        s64 integer;
        f64 floating;
        struct
        {
            struct Literal *elements; // should this be general expressions?
            s64 count;
        };
    };
    Type type;
    // b8 value_as_text; // should this be here or expected by the delexer?
} Literal;

typedef struct Expression
{
    union
    {
        string text;
        Literal literal;
    };
    ExpressionType type;
} Expression;

typedef enum
{
    Global_none,
    Global_globals,
    Global_unknown,
    Global_unknown_token,
    Global_space,
    Global_language_specific,
    Global_expression,
    Global_Declaration,
    Global_eof,
} GlobalType;

typedef struct Global
{
    union
    {
        string text;
        Expression expression;
        Space space;
        struct
        {
            struct Global *elements;
            s64 count;
        } globals;
    };
    GlobalType type;
} Global;