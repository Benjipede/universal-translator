# Structure

This document outlines the structure of the code.

## Contents

- [Includes](#includes)
  - [src/](#src)
  - [lib/](#lib)
- [Interfaces](#interfaces)
  - [Developing Interfaces](#developing-interfaces)
- [support.c](#

## Includes

### lib/

For the purposes of structure the source code files in lib/ are viewed as external libraries.
They provide necessary functionality, but we want to concern ourselves with them as little as possible.
Therefore, every header file in here should contain header guards and they may themselves only include other header files within lib/ or from the standard library.

### src/

If you have had a look at the code in src/ you may wonder the lack of \#include directives and header guards.
This is because the files in src/ are included in a very simple an specific way. To get your head around it begin in [bottleneck.h](src/bottleneck.h).

## Interfaces

By **Interfaces** are meant programs that interface with translator utilities, i.e translators.
They can be user interfaces or interfaces that communicates with other programs.
Right now there is only one interface: [unitrl](src/interfaces/unitrl.c).
This is a command line user interface.

### Developing Interfaces

You are more than welcome to develop interfaces for Universal Translator.

The way files are [included](#Includes) in the 