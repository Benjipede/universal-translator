#include "lexer.h"
#include "delexer.h"
#include "parser.h"
#include "deparser.h"

//
// Just set these values
//

const char *name_c                  = "c";
const char *extensions_c[]          = {"c", "h"};

Lexer const lexer_c_default         = lex_c;
Parser const parser_c_default       = parse_c;
Deparser const deparser_c_default   = deparse_c;
Delexer const delexer_c_default     = delex_c;

//
// Automated from here
//

string extension_array_c[array_count(extensions_c)];

Language get_language_c()
{
    Language language;
    
    language.name = string_from_cstring(name_c);
    language.lexer = lexer_c_default;
    language.parser = parser_c_default;
    language.deparser = deparser_c_default;
    language.delexer = delexer_c_default;
    
    language.extensions = extension_array_c;
    for(s64 index = 0; index < array_count(extension_array_c); ++index)
        language.extensions[index] = string_from_cstring(extensions_c[index]);
    language.extension_count = array_count(extension_array_c);
    
    return language;
}