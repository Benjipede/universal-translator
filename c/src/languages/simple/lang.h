#include "lexer.h"
#include "delexer.h"
#include "parser.h"
#include "deparser.h"

Language get_language_simple()
{
    Language language;
    char *extension_array[] = {"sim"};
    language.name = "simple";
    language.lexer = lex_simple;
    language.parser = parse_simple;
    language.deparser = deparse_simple;
    language.delexer = delex_simple;
    
    language.extensions = extension_array;
    language.extension_count = array_count(extension_array);
    
    return language;
}