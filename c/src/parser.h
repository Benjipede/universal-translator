typedef Global (*Parser)(Lexer, Reader *, string *, Stack *, Queue *);

#include "basic_parser.h"
#include "languages/simple/parser.h"
#include "languages/lambscript/parser.h"