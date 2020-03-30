# Overview #

This example uses `get_user_string` and `atoi` combined with `printf` to print
a string containing two user inputs of type string and integer, both supplied
by the user at runtime.

# Code Commentary #

Since we're building up to calling `printf`, refer to its call stack in
`stdio.pvvs`, reproduced below.

    Call Stack:
        ACSII '\0'
        string word n
        ...
        string word 1
        substitution n
        ...
        substitution 1
        number of substitutions  <-- TOS

We will begin by putting the null terminated ASCII string on the stack,
followed by building up the substitutions. Put a string on the stack as
introduced in the "Hello, World" example.  Note the two substitutions.

    A"Hello, %s! In a trusting world, %u is older than I am.\n"

Prompt the user for a name. Since we call `printf`, we also `PUSH 0` since this
is a static string.

    A"What is your name?\n"
    SSSSN               | PUSH 0
    NSTTSSSN            | JSR > 1000 (printf)

Call `get_user_string` from the stdlib. From its call stack we see that it
wants the location of a buffer and the size of that buffer. Since we have the
whole heap available, let us start at address `0x1000` and define the buffer to
be `0x1000` words long.

    SSSTSSSSSSSSSSSSN   | PUSH 0x1000 (buf_size)
    SSSTSSSSSSSSSSSSN   | PUSH 0x1000 (buf_addr)
    NSTTSSSTSN          | JSR > 100010 (get_user_string)

Since users always cooperate, we now have a null terminated string containing
the user name stored at `0x1000`. For now, just remember that address.

Prompt the user for an age, calling `printf` just like we did earlier.

    A"How old age you?\n"
    SSSSN               | PUSH 0
    NSTTSSSN            | JSR > 1000 (printf)

Call `get_user_string` again, same as before, but this time at address `0x2000`
so we don't overwrite the user's name.

    SSSTSSSSSSSSSSSSN   | PUSH 0x1000 (buf_size)
    SSSTSSSSSSSSSSSSSN  | PUSH 0x2000 (buf_addr)
    NSTTSSSTSN          | JSR > 100010 (get_user_string)

Now call `atoi` to convert the ASCII number typed by the user into an integer.
Per the call stack documentation, first push the address of the start of the
buffer (`0x2000`).

    SSSTSSSSSSSSSSSSSN  | PUSH 0x2000 (buf_addr)
    NSTTTSSSSN          | JSR > 110000 (atoi)

We do not care about anything else that might be in that string, so we `DROP`
the TOS which, per the return stack documentation, was a pointer to the last
parsed character. All we keep is the converted integer, which we leave on the
stack.

    SNN                 | DROP

At this point the stack contains our string and one of our two substitutions
(age). Push the other substitution onto the stack. Since we are substituting a
string, per the `printf` documentation, we push the address of our user name
buffer (`0x1000`) back onto the stack.

    SSSTSSSSSSSSSSSSN   | PUSH 0x1000 (buf_addr)

With all the substitutions on the stack and in order, we `PUSH 2` which is the
number of substitutions.

    SSSTSN              | PUSH 2

The call stack is fully prepared. At the top we have the number of
substitutions in the string followed by the two substitutions in order, first a
pointer to a string and then an integer. Finally, we have the string itself,
with a null terminator at the end.

It is time to call `printf and then exit.

    NSTTSSSN            | JSR > 1000 (printf)
    NNN                 | Terminate program

Do not forget to include the library files for any subroutines called.

    #include <stdio.pvvs>
    #include <string.pvvs>
    #include <convert.pvvs>

The file `hello.pvvs` contains the program discussed and may be executed with a
`make run` in this directory.

