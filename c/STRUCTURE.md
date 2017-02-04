# Structure

This document outlines the structure of the code.

## Contents

- [Terminology](#terminology)
- [Includes](#includes)
  - [lib/](#lib)
  - [src/](#src)
- [Translation](#translation)
  - [Streamers](#streamers)
  - [Languages](#languages)
- [Interfaces](#interfaces)
  - [Developing Interfaces](#developing-interfaces)

## Terminology

Some terminology to reduce the amount of text:

For each supported language there are at least one of each of the following procedure types:

- **Lexer**: `Token lex_name(Reader *reader, string *storage)`
- **Parser**: `Global parse_name(Lexer lexer, Reader *reader, string *storage)`
- **Deparser**: `void deparse_name(Delexer delexer, Writer *writer, Global ast)`
- **Delexer**: `void delex_name(Writer *writer, Token token)`

Procedures of these types are collectively referred to as [**tools**](LANGUAGE.md#tools).

As you can see, lexers takes a pointer to a `Reader`.
Likewise delexers takes a pointer to a `Writer`.
**Readers** and **writers** are collectively referred to as [**streamers**](#streamers).

## Includes

### lib/

For the purposes of structure the source code files in lib/ are viewed as external libraries.
They provide necessary functionality, but we want to concern ourselves with them as little as possible.
Therefore, every header file in here should contain header guards and they may themselves only include other header files within lib/ or from the standard library.

### src/

If you have had a look at the code in src/ you may wonder the lack of \#include directives and header guards.
This is because the files in src/ are included in a very simple an specific way. To get your head around it begin in [bottleneck.h](src/bottleneck.h).

## Translation

A translation goes through six different step:

1. A source file is read by a reader.
2. A lexer for some language makes tokens from the characters it obtains from the reader.
3. Then a parser for the same language makes an abstract syntax tree from these tokens.
4. The abstract syntax tree is then broken down into tokens by a deparser for some other language.
5. A delexer for second language then breaks down these new tokens into characters.
6. Finally, the delexer outputs the characters through a writer.

### Streamers

The code for streamers exists in [src/streamers](src/streamers).

### Languages

For more information on how languages are handled and how to contribute by expanding language support see [LANGUAGES](LANGUAGES.md).

## Interfaces

By **Interfaces** are meant programs that interface with translator utilities, i.e translators.
They can be user interfaces or interfaces that communicates with other programs.
The source code for interfaces exists in sr/interfaces/.
Right now there is only one interface: [unitrl](src/interfaces/unitrl.c).
This is a command line user interface.

### Developing Interfaces

You are more than welcome to develop interfaces for Universal Translator.
The way [includes](#Includes) are handled means that you only have to include bottleneck.h to have access to all utilities of Universal Translator.

Put the source code for your interface in src/interface/.

**To avoid clutter in the case that others decide to develop their own interfaces use only one file or put all files in a subfolder.**

## support

[support](src/support.c) is a program found in [misc/](misc) that creates and updates support information of programming languages.