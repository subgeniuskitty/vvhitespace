VVhitespace is descended from Whitespace, adding a vertical tab to the language
along with some restrictions to ease implementation. The name is intended to
embrace the spirit of Whitespace's visual incomprehensibility.

Since Whitespace ignores [VTab] as a comment character, and since the
Whitespace VM is a superset of the VVhitespace VM, all valid VVhitespace
programs are also valid Whitespace programs.

Locating a Whitespace interpreter capable of running all valid Whitespace
programs is left as an exercise for the reader. For the inquisitive, the
following program, read top-down, is both a valid VVhitespace and Whitespace
program. Does it terminate or infinitely loop?

[Space]
[Space]
[Space]
[Tab]
[LF]
[LF]
[Space]
[LF]
[Space]
[Tab]
[LF]
[VTab]
[Space]
[Tab]
[LF]
[LF]
[LF]
[LF]
