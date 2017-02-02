#include "lexer.h"
#include "delexer.h"
#include "parser.h"
#include "deparser.h"

const char *name_simple                  = "simple";
const char *extensions_simple[]          = {"sim"};

Lexer const lexer_simple_default         = lex_simple;
Parser const parser_simple_default       = parse_simple;
Deparser const deparser_simple_default   = deparse_simple;
Delexer const delexer_simple_default     = delex_simple;

//
// Automated from here
//

string extension_array_simple[array_count(extensions_simple)];

Language get_language_simple()
{
    Language language;
    
    language.name = string_from_cstring(name_simple);
    language.lexer = lexer_simple_default;
    language.parser = parser_simple_default;
    language.deparser = deparser_simple_default;
    language.delexer = delexer_simple_default;
    
    language.extensions = extension_array_simple;
    for(s64 index = 0; index < array_count(extension_array_simple); ++index)
        language.extensions[index] = string_from_cstring(extensions_simple[index]);
    language.extension_count = array_count(extension_array_simple);
    
    return language;
}