# Structure

This document outlines the structure of the code.

## Contents

- [Includes](#includes)
  - [src/](#src)
  - [lib/](#lib)
- [Translation](#translation)
  - [Tools](#tools)
  - [languages/](#languages)
- [Streamers](#streamers)
- [Interfaces](#interfaces)
  - [Developing Interfaces](#developing-interfaces)
- [support](#support)

## Includes

### lib/

For the purposes of structure the source code files in lib/ are viewed as external libraries.
They provide necessary functionality, but we want to concern ourselves with them as little as possible.
Therefore, every header file in here should contain header guards and they may themselves only include other header files within lib/ or from the standard library.

### src/

If you have had a look at the code in src/ you may wonder the lack of \#include directives and header guards.
This is because the files in src/ are included in a very simple an specific way. To get your head around it begin in [bottleneck.h](src/bottleneck.h).

## Translation

### Tools



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