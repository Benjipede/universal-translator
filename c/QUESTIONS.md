# Questions

In this document I ask questions that I am considering myself and welcome more experienced programmers to answer for me.

## Questions that need answers

- Does the current 'division of labor' make sense?
More specifically, is it a good idea abstract streaming away from lexing and delexing procedures?
My reason for doing so is to more easily enable different encodings and possibly streaming from other things than files. It also forces some standardization the code across lexers and delexers, which I think is valuable as it makes the code more comprehensible in case the project gets multiple contributors.
- Does the different [tools](c/languages/LANGUAGES.md#tools) have access to the correct data structures and are the conventions for their uses the right ones?
I definitely expect this to change multiple times, but your advice could maybe cut down the number of iterations before stagnation.
- As I understand, in the past UTF-8 characters were allowed to be up to six bytes in size, but in 2003 this number was shrunk to four bytes to be compatible with UTF-16 (thanks to Microsoft once again making a bad desition in regards to encoding). My question is: Should the character encoding (not inside strings of course) internal to the program be UTF-8 or UTF-32? My initial answer is to go with UTF-32 as I expect the number of code points of UTF-16 to be too few in the future and UTF-8 or whatever encoding we will be using at that time cannot be kept down to four bytes in size while still being memory efficient.
- I have pretty much decided to change from unions to inheritance and from passing values to passing pointers.
I think this will also make also make data management easier through the use of the dynamic array struct [PointerArray](lib/pointer_array.h) so I will make the jump soon unless you have compelling arguments against doing so.

## Questions that has been answered