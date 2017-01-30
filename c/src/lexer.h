typedef Token (*Lexer)(Reader *, string *);

#include "basic_lexer.h"

#include "languages/simple/lexer.h"
#include "languages/lambscript/lexer.h"