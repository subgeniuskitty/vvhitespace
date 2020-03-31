# Overview #

Welcome to VVhitespace!

If you are impatient to get started, simply execute `make` in the top-level
directory to build the compiler and interpreter, then move to one of the
example directories like `examples/wumpus` and execute `make run`.

VVhitespace is descended from Whitespace, adding a vertical tab to the language
along with some restrictions to ease implementation. VVhitespace code is
represented with the *whitespace characters* `[Tab]`, `[VTab]`, `[Space]`, and
`[Newline]`. All other characters are considered commentary.

This repository includes several distinct parts:

  - A compiler, `vvc`, accepts human-readable VVhitespace source code and
    translates it to true VVhitespace code.

  - An interpreter, `vvi`, accepts true VVhitespace files as generated by `vvc`
    and executes them according to the rules in `reference.md`.

  - A library of useful functions including enhanced stack operations, printf,
    math operations including random number generation, heap operations, string
    manipulations and user interactions, and bitwise logic functions. See the
    `stdlib/README.md` for more details.

  - Hunt the Wumpus, a text game in which you explore a randomly connected set
    of caverns, avoiding deep pits and giant cave bats, all while shooting
    arrows at a Wumpus that wants to eat a tasty, meaty human like yourself! The
    cave entrance is rumored to be near `examples/hunt-the-wumpus/README.md`.

  - Examples including code commentary that demonstrate VVhitespace and the
    stdlib. See individual subdirectories under `examples/` for details.

  - Tests for both the VVhitespace language and the stdlib. Intended to
    increase confidence for users that want to tinker with the internals, both
    test suites come with a README explaining how to run the tests and extend them
    as necessary. To execute all tests, simply run `make test` in the top level
    directory.

From this point forward, READMEs will assume you have a basic knowledge of
VVhitespace (or at least Whitespace). The file `reference.md` provides a short
but comprehensive language reference copied largely from the original
Whitespace language tutorial.


# Status #

Complete. Tested on FreeBSD and Debian Linux (w/'build-essential' package).


# Instructions #

To build the VVhitespace software, simply run `make` in the top level directory.
This will produce two binaries, `vvc` for compiling human-readable
pseudo-VVhitespace into true VVhitespace, and `vvi` for interpreting/executing
VVhitespace programs.

Use these two programs to build and run your VVS programs:

    vvc -i your_code.pvvs -o output_file.vvs
    vvi -i output_file.vvs

By convention, the extension `.pvvs` is used for pseudo-VVhitespace code and
`.vvs` is used for true VVhitespace code. 

The stdlib uses the C preprocessor to `#include` library files. Projects like
`examples/hello-world` demonstrate combining the stdlib and the C preprocessor
in the `Makefile`. The invocation will take this general form:

    cpp -I/path/to/stdlib/files -I. -o temp.pvvs your_code.pvvs
    vvc -i temp.pvvs -o output_file.vvs
    vvi -i output_file.vvs

Alternatively, you may simply hijack one of the example projects. All are
pre-configured to compile with the standard library using `make run` and only
require the user to `#include` the appropriate stdlib files for the functions
called. For example, if the `printf` function was used, the bottom of the file
should include this line:

    #include <stdio.pvvs>

Before running the VVhitespace or stdlib tests, read `README.md` in their
respective directories. It details the steps required to configure the tests.
Every test is also a minimal example for a single VVhitespace feature or stdlib
function, with documented input and expected output.


# Helpful Hints #

Syntax highlighting greatly eases both reading and writing VVhitespace code.
Examples for `vim` (and any other editor with regex based syntax highlighting)
can be found in `syntax_highlighting/README.md`.

In addition to Hunt the Wumpus, the `examples/` directory provides several
other smaller examples. All have comprehensible `Makefile` and code that is
easily modified to use as the start of your own project.


# Backward Compatibility #

Since the `[VTab]` character is considered a comment character in Whitespace,
most VVhitespace code should also be valid Whitespace code, and usually with
similar results. All other changes to the language attempt to restrict
implementation impediments without violating the original definition.

Tests are included for both the core VVhitespace language and the stdlib. After
compiling with `cpp` and `vvc`, the resulting tests may be executed with any
Whitespace compiler to identify any specific incompatibilities. Everything has
been kept simple in the hope it will be easy to modify.

