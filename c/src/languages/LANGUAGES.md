# Language Guidelines

Follow these guidelines when you want to change anything in this folder.

## Contents

- [Priorities](#priorities)
- [Structure](#structure)
  - [Subfolders](#subfolders)
  - [lang.h](#langh)
  - [lexer.h](#lexerh)
  - [parser.h](#parserh)
  - [deparser.h](#deparserh)
  - [delexer.h](#delexerh)
- [Extend support of a language](#extend-support-of-a-language)
  - [Create a new tool for an existing language](#create-a-new-tool-for-an-existing-language)
- [Add support for new language](#add-support-for-new-language)

## Priorities

- [ ] Nail down the structure nailed down
- [ ] Support a few languages more fully for testing purposes

## Structure

**Warning: The structure is subject to change, so expect that your contributions will need to be updated.**

languages.h acts as the bottleneck of the "languages" folder. It defines some types, includes */lang.h for every subfolder *, defines an array of `Language get_language_*()` functions included from subfolders.

### Subfolders

Each subfolder represents a programming language. Each of them contains at least the files lang.h, lexer.h, parser.h, deparser.h, and delexer.h.

Example subfolder:
> name/:

> lang.h lexer.h  parser.h  deparser.h  delexer.h

### lang.h

lang.h acts as the bottleneck of its subfolder. It includes [the other files](subfolders). Furthermore, it defines the `Language get_language_*()` function for the language which returns some information about the language, which at the time of this writing is the name of the language, possible extensions and the default lexer, parser, deparser and delexer (see example below). The first extension returned is used for extension-inference by the command-line interface.

Example lang.h:
```c
#include lexer.h
#include delexer.h
#include parser.h
#include deparser.h

Language get_language_name()
{
    Language language;
    char *extension_array[] = {"name_primary_extension", "name_header_file"}; // Argh! Why does 'name' have header files and why are the file extensions so verbose??!!
    language.name = "name";
    language.lexer = lex_name;
    language.parser = parse_name;
    language.deparser = deparse_name;
    language.delexer = delex_name;
    
    language.extensions = extension_array;
    language.extension_count = array_count(extension_array);
    
    return language;
}
```

### lexer.h

lexer.h defines one or more procedures of the form `Token lex_name(Reader *reader, string *storage)`. `lex_name` reads source code through `reader` (see "..\reader.h") and returns a token. It uses `storage` to store strings and other variable-length data.

Example lexer.h:

### parser.h

parser.h defines one or more procedures of the form `Global parse_name(Lexer lexer, Reader *reader, string *storage)`. `parse_name` lexes tokens by calling `lexer(reader, storage)` and returns an abstract syntax tree. Like `lex_name` it also uses `storage` to store data.

Example parser.h:

### deparser.h

deparser.h defines one or more procedures of the form `void deparse_name(Delexer delexer, Writer *writer, Global ast)`. `deparse_name` breaks down `ast` into the tokens that would naturally make up such an abstract syntax tree in the specific language and passes each of them to `delexer` together with `writer`.

Example deparser.h:

### delexer.h

delexer.h defines one or more procedures of the form `void delex_name(Writer *writer, Token token)`. `delex_name` breaks down `token` into characters would naturally make up such a token in the specific language and writes them through `writer`.

Example delexer.h:

**The structure is still subject to change. If you have advice on the structure please let me know.**

## Extend support of a language

We call the aforementioned procedures in [lexer.h](#lexerh), [parser.h](#parserh), [deparser.h](#deparserh), and [delexer.h](#delexerh) tools.

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