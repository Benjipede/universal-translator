# Universal Translator
Welcome Contributor!

Universal Translator is a translator of programming languages that translate partially between not fully supported languages.
Unsupported language features are simply translated into marked comments.
Comments and white space are also translated.

## Goal
The primary goal is to be able to avoid most of the drudge work of translating source code from one language to another by having this program do a simple direct translation. To acheive this I aim to make the code highly modular to make it easy to add support for new languages and make it possible to stream code in and out in different ways. Later, the program might be able to use exclusive features of languages, but generally a programmer will do a much better job at tailoring translated code to the strenghs of a target language, so this is not a high priority.

The code exists in the "c/" folder since I hope to port the code to Jonathan Blow's programming language JAI when it is released as it has many features that will be very useful for this project.

## Contribute
At this early stage I am trying to nail down how the code should be structured. If you are experienced with lexing, parsing etc., then I will appreciate your advice. I welcome all to help make language **tools**, i.e **lexers**, **parsers**, **deparsers**, and **delexers**. You can also help with developing **readers** and **writers**, i.e. tools that stream code from and to the outside world (files for now), collectively known as **streamers**. Finally I want support for unicode characters and UTF-8 strings as I want these to be the internal encodings in the program to be able to get maximal support. Although early contributions are likely to need updates, they will be valuable for testing.
You can find more detailed descriptions in the links below.

- If you want to develop language tools see [LANGUAGES](c/src/languages/LANGUAGES.md) if you would like to contribute.
- If you want to develop streamers.
- If you want to help support unicode characters and UTF-8 strings see [lib/unicode.h](c/lib/unicode.h).
- To get a more detailed todo-list see [TODO](c/TODO.md).
- For a more detailed overview of the code structure see [TODO](c/TODO.md).

I plan to soon submit Universal Translator as a project to [Handmade Network](https://handmade.network/) to get a progress blog and a forum, so if this project has your interest look out for it there.

Good day to you