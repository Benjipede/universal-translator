# Universal Translator
A translator of programming languages that translate partially between not fully supported languages.
Unsupported language features are simply translated into marked comments.
Comments and white space are also translated.

## Goal
The primary goal is to be able to avoid most of the drudge work of translating source code from one language to another by having this program do a simple direct translation. To acheive this I aim to make the code highly modular to make it easy to add support for new languages and make it possible to stream code in and out in different ways. Later, the program might be able to use exclusive features of languages, but generally a programmer will do a much better job at tailoring translated code to the strenghs of a target language, so this is not a high priority.

The code exists in the "c/" folder since I hope to port the code to Jonathan Blow's programming language JAI when it is released as it has many features that will be very useful for this project.

## Contribute
At this early stage I am trying to nail down how the code should be structured. If you are experienced with lexing, parsing etc., then I will appreciate your advice. I also hope that some will help make lexers, parsers etc. for some languages. Although early contributions are likely to need updates, they will be valuable for testing. See [c/src/languages/LANGUAGES.md](c/src/languages/LANGUAGES.md) if you would like to contribute.

I plan to soon submit Universal Translator as a project to [Handmade Network](https://handmade.network/) to get a progress blog and a forum, so if this project interests you look out for it appearing there.

Good day to you