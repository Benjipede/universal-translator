typedef enum
{
    Token_none,
    Token_error,
    Token_unsupported,
    Token_unknown,
    Token_whitespace,
    Token_comment,
    Token_identifier,
    Token_literal,
    Token_semicolon,
    Token_sof,
    Token_eof,
} TokenKind;

typedef struct
{
    TokenKind type;
} Token;

typedef struct
{
    Token self;
    string text;
}

typedef struct
{
    Token self;
    s64 line_feed_count;
    s64 space_count;
} TokenWhitespace

typedef struct
{
    TokenText self;
    CommentKind kind;
} TokenComment;

typedef struct
{
    TokenText self;
    LiteralType type;
} TokenLiteral;

typedef struct
{
    TokenLiteral self;
    s64 BitSize;
} TokenNumberLiteral;