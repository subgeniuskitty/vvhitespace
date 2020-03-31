# Overview #

This example starts with the `Hello, world!` example from Wikipedia's
Whitespace page, gradually rewriting it to use the ASCII string feature of the
VVhitespace compiler and the `printf` subroutine from the stdlib.

# Code Commentary #

Wikipedia's `Hello, World!` example is reproduced below. It follows a simple
form, pushing an integer corresponding to an ASCII character onto the stack and
then printing it with an IO instruction, repeating this two part process for
each character before terminating with the final instruction.

    SSSTSSTSSSN     | Push +72 (ASCII H)
    TNSS            | Output character
    SSSTTSSTSTN     | Push +101 (ASCII e)
    TNSS            | Output character
    SSSTTSTTSSN     | Push +108 (ASCII l)
    TNSS            | Output character
    SSSTTSTTSSN     | Push +108 (ASCII l)
    TNSS            | Output character
    SSSTTSTTTTN     | Push +111 (ASCII o)
    TNSS            | Output character
    SSSTSTTSSN      | Push +44 (ASCII ,)
    TNSS            | Output character
    SSSTSSSSSN      | Push +32 (ASCII space)
    TNSS            | Output character
    SSSTTTSTTTN     | Push +119 (ASCII w)
    TNSS            | Output character
    SSSTTSTTTTN     | Push +111 (ASCII o)
    TNSS            | Output character
    SSSTTTSSTSN     | Push +114 (ASCII r)
    TNSS            | Output character
    SSSTTSTTSSN     | Push +108 (ASCII l)
    TNSS            | Output character
    SSSTTSSTSSN     | Push +100 (ASCII d)
    TNSS            | Output character
    SSSTSSSSTN      | Push +33 (ASCII !)
    TNSS            | Output character
    SSSTSTSN        | Push +10 (ASCII '\n')
    TNSS            | Output character
    NNN             | Terminate program

If we push the string onto the stack in reverse order we can rearrange the
code as shown below. Now we're first pushing all the ASCII characters onto the
stack, then printing them all at once.

    SSSTSTSN        | Push +10 (ASCII '\n')
    SSSTSSSSTN      | Push +33 (ASCII !)
    SSSTTSSTSSN     | Push +100 (ASCII d)
    SSSTTSTTSSN     | Push +108 (ASCII l)
    SSSTTTSSTSN     | Push +114 (ASCII r)
    SSSTTSTTTTN     | Push +111 (ASCII o)
    SSSTTTSTTTN     | Push +119 (ASCII w)
    SSSTSSSSSN      | Push +32 (ASCII space)
    SSSTSTTSSN      | Push +44 (ASCII ,)
    SSSTTSTTTTN     | Push +111 (ASCII o)
    SSSTTSTTSSN     | Push +108 (ASCII l)
    SSSTTSTTSSN     | Push +108 (ASCII l)
    SSSTTSSTSTN     | Push +101 (ASCII e)
    SSSTSSTSSSN     | Push +72 (ASCII H)
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    NNN             | Terminate program

As a convenience, the VVhitespace compiler will accept `A""` syntax, generating
a sequence of `PUSH` instructions, just like the prior examples, that places a
null-terminated ASCII string on the stack. This allows us to shorten the
program. Like before, the start of the string is closest to the top of the
stack. Unlike before, there is now a null terminator at the end of the string.
For now, we'll just leave that on the stack when the program terminates.

    A"Hello, world!\n"
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    TNSS            | Output character
    NNN             | Terminate program

Finally, we will use `printf` from the stdlib to shorten that mass of IO
instructions. Every stdlib subroutine includes a text description as well as
diagrams of the call and return stack. From `stdio.pvvs` we see the call stack
for `printf` reproduced below.

    Call Stack:
        ACSII '\0'
        string word n
        ...
        string word 1
        substitution n
        ...
        substitution 1
        number of substitutions  <-- TOS

The text description for `printf` also informs us that, if we are printing a
static string (i.e. no substitutions like '%d'), the `number of substitutions`
should be `0` and it should be immediately followed by `string word 1`. Looking
at the call stack, that makes sense since we have no substitutions to place
between those two things.

Note the ASCII `\0` on the call stack. That corresponds to the null terminator
at the end of our string that was automatically placed by the `A""` syntax.

Don't forget to `#include` the relevant stdlib files. This is done at the
bottom of the file since VVhitespace files are executed from top to bottom. You
wouldn't want to accidentally start executing random library code!

Putting all that together, we discover a greatly simplified program.

    A"Hello, world!\n"
    SSSSN           | PUSH 0 (number of substitutions)
    NSTTSSSN        | JSR > 1000 (stdlib:printf)
    NNN             | Terminate program
    #include <stdio.pvvs>

This is already saved in `hello.pvvs` which you can execute with `make run`.
