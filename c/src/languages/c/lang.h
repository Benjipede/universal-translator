#include "lexer.h"
#include "delexer.h"
#include "parser.h"
#include "deparser.h"

Language get_language_c()
{
    Language language;
    char *extension_array[] = {"c", "h"};
    language.name = "c";
    language.lexer = lex_c;
    language.parser = parse_c;
    language.deparser = deparse_c;
    language.delexer = delex_c;
    
    language.extensions = extension_array;
    language.extension_count = array_count(extension_array);
    
    return language;
}