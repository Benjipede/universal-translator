# Language Guidelines

This document outlines the structure of the code in this folder and gives guidlines regarding contributions to language support.

## Contents

- [Terminology](#terminology)
- [Structure](#structure)
  - [Subfolders](#subfolders)
  - [lang.h](#langh)
  - [Tools](#tools)
    - [lexer.h](#lexerh)
    - [parser.h](#parserh)
    - [deparser.h](#deparserh)
    - [delexer.h](#delexerh)
- [Extend support of a language](#extend-support-of-a-language)
  - [Create a new tool for an existing language](#create-a-new-tool-for-an-existing-language)
- [Add support for new language](#add-support-for-new-language)

## Terminology

For each supported language there are at least one of each of the following procedure types:

- **Lexer:** `Token lex_name(Reader *reader, string *storage)`
- **Parser:** `Global parse_name(Lexer lexer, Reader *reader, string *storage)`
- **Deparser:** `void deparse_name(Delexer delexer, Writer *writer, Global ast)`
- **Delexer:** `void delex_name(Writer *writer, Token token)`

Procedures of these types are collectively referred to as **tools**.

## Structure

**Warning: The structure is subject to change, so expect that your contributions will need to be updated.**

languages.h acts as the bottleneck of the "languages" folder. It defines some types, includes */lang.h for every subfolder *, defines an array of `Language get_language_*()` functions included from subfolders, and defines some language look-up functions.

### Subfolders

Each subfolder represents a programming language. Each of them contains at least the files lang.h, lexer.h, parser.h, deparser.h, and delexer.h. A subfolder name may not contain uppercase letters.

Sample subfolder:
> name/:

> lang.h lexer.h  parser.h  deparser.h  delexer.h

### lang.h

lang.h acts as the bottleneck of its subfolder. It includes [the other files](subfolders). Furthermore, it defines the `Language get_language_*()` function for the language which returns some information about the language, which at the time of this writing is the name of the language, possible extensions and the default tools (see example below). The primary extension is returned as the first possible extension. This means that it can be used for extension-inference by interfaces (see [unitrl](../interfaces/unitrl.c)).

Sample lang.h:
```c
#include lexer.h
#include delexer.h
#include parser.h
#include deparser.h

//
// Just set these values
//

const char *name_name                  = "name";
const char *extensions_name[]          = {"name_primary_extension", "name_header_file"}; // Argh! Why does 'name' have header files and why are the file extensions so verbose??!!

Lexer const lexer_name_default         = lex_name;
Parser const parser_name_default       = parse_name;
Deparser const deparser_name_default   = deparse_name;
Delexer const delexer_name_default     = delex_name;

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
```

### Tools

#### lexer.h

lexer.h defines one or more procedures of the form `Token lex_name(Reader *reader, string *storage)`. `lex_name` reads source code through `reader` (see "..\reader.h") and returns a token. It uses `storage` to store strings and other variable-length data.

Sample lexer.h:

#### parser.h

parser.h defines one or more procedures of the form `Global parse_name(Lexer lexer, Reader *reader, string *storage)`. `parse_name` lexes tokens by calling `lexer(reader, storage)` and returns an abstract syntax tree. Like `lex_name` it also uses `storage` to store data.

Sample parser.h:

#### deparser.h

deparser.h defines one or more procedures of the form `void deparse_name(Delexer delexer, Writer *writer, Global ast)`. `deparse_name` breaks down `ast` into the tokens that would naturally make up such an abstract syntax tree in the specific language and passes each of them to `delexer` together with `writer`.

#### delexer.h

delexer.h defines one or more procedures of the form `void delex_name(Writer *writer, Token token)`. `delex_name` breaks down `token` into characters would naturally make up such a token in the specific language and writes them through `writer`.

**The structure is still subject to change. If you have advice on the structure please let me know.**

## Extend support of a language

We call the aforementioned procedures in [lexer.h](#lexerh), [parser.h](#parserh), [deparser.h](#deparserh), and [delexer.h](#delexerh) tools.

To extend support of a language you simply augment the existing existing tools for that language to handle more cases.

### Create a new tool for an existing language

Maybe you simply want to create a lexer, parser, deparser or delexer. Then just follow step 3, 4, 5, or 6 from the list [below](#add-support-for-new-language) with 'name' appended with a descriptive identifier.

## Add support for new language

When you want to add support for language "name" go through the following steps.

1. Create the subfolder "name", preferably in lowercase letters.
2. In the newly created folder make the files lang.h, lexer.h, parser.h, deparser.h, and delexer.h.
3. In lexer.h put a procedure `Token lex_name(Reader *, string *)` (see [lexer.h](#lexerh)).
4. In parser.h put a procedure `Global parser_name(Lexer, Reader *, string *, Stack *, Queue *)`  (see [parser.h](#parserh)).
5. In deparser.h put a procedure `void deparser_name(Delexer, Writer *, Global)`  (see [deparser.h](#deparserh)).
6. In delexer.h put a procedure `void delexer_name(Writer *, Token)`  (see [delexer.h](#delexerh)).
7. In lang.h put includes to the other files and a procedure `Language get_language_name()`  (see [lang.h](#langh)).