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
- [Expand support of a language](#expand-support-of-a-language)
  - [Create a new tool for an existing language](#create-a-new-tool-for-an-existing-language)
- [Add support for new language](#add-support-for-new-language)
- [Support Information](#support-information)
  - [Tokens](#tokens)
  - [Code Nodes](#code-notes)
  - [Marking Tools](#marking-tools)
  - [Generating Support Information](#generating-support-information)

## Terminology

See [STRUCTURE](../../STRUCTURE.md#terminology).

## Structure

**Warning: The structure is subject to change, so expect that your contributions will need to be updated.**

_Please let me know if you can give some advice on the structure._

languages.h acts as the bottleneck of the "languages" folder.
It defines some types, includes */lang.h for every subfolder *,
defines an array of `Language get_language_*()` functions included from subfolders,
and defines some language look-up functions.

### Subfolders

Each subfolder represents a programming language.
Each of them contains at least the files lang.h, lexer.h, parser.h, deparser.h, and delexer.h.
A subfolder name may not contain uppercase letters.

Sample subfolder:
> name/:

> lang.h lexer.h  parser.h  deparser.h  delexer.h

### lang.h

lang.h acts as the bottleneck of its subfolder.
It includes [the other files](subfolders).
Furthermore, it defines the `Language get_language_*()` function for the language which returns some information about the language,
which at the time of this writing is the name of the language, possible extensions and the default [tools](#tools) (see example below).
The primary extension is returned as the first possible extension.
This means that it can be used for extension-inference by interfaces (see [unitrl](../interfaces/unitrl.c)).

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

lexer.h defines one or more procedures of the form `Token lex_name(Reader *reader, Pool *pool)`.
`lex_name` reads source code through `reader` (see "..\reader.h") and returns a token.
It uses `pool` to store strings and other variable-length data.

**Note that [basic_lexer.h](../basic_lexer.h) contains helper-procedures for lexing.**

Sample lexer.h:

#### parser.h

parser.h defines one or more procedures of the form `Global parse_name(Lexer lexer, Reader *reader, Pool *pool, Stack *stack, Queue *que)`.
`parse_name` lexes tokens by calling `lexer(reader, pool)` and returns an abstract syntax tree.
Like `lex_name` it also uses `pool` to store data.

The convensions

**Note that [basic_parser.h](../basic_parser.h) contains helper-procedures for parsing.**

_Consider: Are a stack and a queue the right data structures for a parser?_

Sample parser.h:

#### deparser.h

deparser.h defines one or more procedures of the form `void deparse_name(Delexer delexer, Writer *writer, Global ast)`.
`deparse_name` breaks down `ast` into the tokens that would naturally make up such an abstract syntax tree in the specific language and passes each of them to `delexer` together with `writer`.

**Note that [basic_deparser.h](../basic_parser.h) contains helper-procedures for deparsing.**

_Consider: Should deparsers have access to a pool and/or some data structures?_

#### delexer.h

delexer.h defines one or more procedures of the form `void delex_name(Writer *writer, Token token)`.
`delex_name` breaks down `token` into characters would naturally make up such a token in the specific language and writes them through `writer`.

**Note that [basic_delexer.h](../basic_delexer.h) contains helper-procedures for delexing.**

_Consider: Should delexers have access to a pool and/or some data structures?_

## Expand support of a language

To expand support of a language you simply augment the existing tools for that language to handle more cases.

### Create a new tool for an existing language

Maybe you simply want to create a tool from stratch. Then just follow step 3, 4, 5, or 6 from the list [below](#add-support-for-new-language) with 'name' appended with a descriptive identifier.

## Add support for new language

When you want to add support for language "name" go through the following steps.

1. Create the subfolder "name", preferably in lowercase letters
(or when implemented: run 'support -new name' from the command line which will automatically do step 1 and 2).
2. In the newly created folder make the files lang.h, lexer.h, parser.h, deparser.h, and delexer.h.
3. In lexer.h put a procedure `Token lex_name(Reader *, Pool *)` (see [lexer.h](#lexerh)).
4. In parser.h put a procedure `Global parser_name(Lexer, Reader *, Pool *, Stack *, Queue *)`  (see [parser.h](#parserh)).
5. In deparser.h put a procedure `void deparser_name(Delexer, Writer *, Global)`  (see [deparser.h](#deparserh)).
6. In delexer.h put a procedure `void delexer_name(Writer *, Token)`  (see [delexer.h](#delexerh)).
7. In lang.h put includes to the other files and a procedure `Language get_language_name()`  (see [lang.h](#langh)).

## Support Information

Preferably, a subfolder also contains the files LANG.md and SUPPORT.md.
LANG should contain at least the headlines '## Tokens' and '## Code Notes'.
Their sections should adhere to specific formats as described below since they will be used by the command line utility program [support](../support.c) to generate SUPPORT, which will contain information on the currently supported features of the language.

A short description of the language at the top of LANG would be nice, but it is not required in any way.

### Tokens

The text in the tokens section list all the kinds of tokens the language has.

The name of a kind of token can contain characters of all types except some types of whitespace characters, though **commas** have to be escaped with backslashes.
Backslashes can also be used to write line feeds, tabs, and carriage returns in the way you know and love.
Furthermore, any spaces following a backslash are ignored.
Otherwise '\*' will evaluate to '*' for any character * as you would expect.
Note that Hashtags should also be escaped if at the beginning of a new line.

Names are separated by commas and line feeds. Multiple separators in a row are allowed.

### Code Notes

The text in the code notes section lists all the kinds of code notes the language has including what tokens and other code nodes they can contain.

The name of a kind of code note can contain characters of all types except some types of whitespace characters, though **colons** have to be escaped with backslashes.
Otherwise everything works as described [above](#tokens).

Names should be followed by a semicolon and then the kinds of tokens and other code nodes that it can possibly contain. Here the format for [listing token names](#tokens) apply for both tokens and code nodes. This means **commas** rather than **colons** need escaping. However, after a line feed a new code node declaration is expected unless it is followed by a semicolon.

### Marking Tools

In order to generate SUPPORT the line following a tool procedure should read SUPPORT(...).

After a lexer or a delexer SUPPORT(...) should list the kinds of tokens the procedure currently supports.
After a parser or a deparser SUPPORT(...)  should list the kinds of code nodes the procedure currently supports.

Here the listing format is as with [tokens](#tokens) except that close parentheses ')' also has to be escaped.

### Generating Support Information

Run 'support name' from the command line to generate SUPPORT for language 'name'. The top of the file will the contain a marked list of all kinds of code nodes where the supported kinds are checked.
In the same fashion it will also contain the support information of every existing tool for the language.
Later, the you will be able to simply run support without any arguments to generate support information for all languages at once.