typedef enum
{
    Comment_single,
    Comment_multi,
} CommentType;

typedef enum {
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
} TokenType;

typedef struct Token {
    union
    {
        string text;
        struct
        {
            string text;
            CommentType type;
        } comment;
        struct
        {
            s64 newline_count, space_count;
        };
    };
    TokenType type;
} Token;



typedef struct Stack
{
    Token *elements;
    s64 count;
    s64 capacity;
} Stack;

Stack make_stack(Token *elements, s64 capacity)
{
    Stack result;
    result.elements = elements;
    result.count = 0;
    result.capacity = capacity;
    return result;
}

b8 stack_is_empty(Stack stack)
{
    return stack.count == 0;
}

b8 push(Stack *stack, Token token)
{
    if(stack->count >= stack->capacity)  return 0;
    stack->elements[stack->count++] = token;
    return 1;
}

Token pop(Stack *stack)
{
    Token token;
    if(stack->count <= 0)
        token.type = Token_none;
    else
        token = stack->elements[--stack->count];
    return token;
}


typedef struct Queue
{
    Token *elements;
    s64 first_index;
    s64 count;
    s64 capacity;
} Queue;

Queue make_queue(Token *elements, s64 capacity)
{
    Queue result;
    result.elements = elements;
    result.first_index = 0;
    result.count = 0;
    result.capacity = capacity;
    return result;
}

b8 queue(Queue *que, Token token)
{
    if(que->count >= que->capacity)  return 0;
    que->elements[(que->first_index + que->count++) % que->capacity] = token;
    return 1;
}

Token dequeue(Queue *que)
{
    Token token;
    if(que->count <= 0)
        token.type = Token_none;
    else
    {
        token = que->elements[que->first_index];
        que->first_index = (que->first_index + 1) % que->capacity;
        --que->count;
    }
    return token;
}