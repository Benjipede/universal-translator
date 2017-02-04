# Universal Translator

Universal Translator is a tool for translating source code between programming languages that aims to make translations as drugde-free as possible.
Therefore, it not only translates the code, but also comments and whitespace.
Unsupported language features and syntax errors are translated into marked comments.
This means that even partial support for translating to/from a specific language is useful and will significantly reduce drugde work.

## Goal
The primary goal is to be able to avoid most of the drudge work of translating source code from one language to another by having this program do a simple direct translation so that you as a programmer can focus on changing your code to better match the strengths of your target language rather than using a lot of time on reworking the syntax to be able to compile.
To acheive this I aim to make the code reasonably modular, hopefully without major performance hits, to make it easy to add support for new languages and make it possible to stream code in and out in different ways.
For now the priority is to just make , but generally a programmer will do a much better job at tailoring translated code to the strenghs of a target language.
This migth even be an enjoyable experience if the target language is a much better match for the project (or much nicer in general) than the source language, so this is not a priority.

The code exists in the "c/" folder since I hope to port the code to Jonathan Blow's programming language JAI when it is released as it has many features that will be very useful for this project.

## Contributing
At this early stage I am trying to nail down how the code should be structured. I have little experience with lexing and parsing so If you are experienced with it, I will appreciate your advice. See [QUESTIONS](c/QUESTIONS.md) for specific questions I am considering and would like answers to.

I welcome all to help expand support for different languages. You can also help with developing streaming.
Finally I want support for UTF-32 and UTF-8 strings as I want these to be the internal encodings in the program.
Although early contributions are likely to need updates, they will be valuable for testing.
You can find more detailed descriptions in [STRUCTURE](c/STRUCTURE.md) and a short overview of todos in [TODO](c/TODO.md).

I plan to soon submit Universal Translator as a project to [Handmade Network](https://handmade.network/) to get a progress blog and a forum, so if this project has your interest look out for it there.

Good day to you