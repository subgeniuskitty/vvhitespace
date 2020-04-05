# Overview #

This folder contains a library of useful functions written in VVhitespace.
They are intended to remove the tedium of frequently repeated patterns while
remaining short enough to easily comprehend and modify.

For those eager to jump right in, all functions have comments in the source
code containing a text description as well as both call and return stacks.
Simply `#include` the relevant file, setup your call stack per the
documentation and then `JSR` to the stdlib function of your choice.

Some functions, like `deepdup`, `stackrotate` and `stackrotatereverse`, ease
stack manipulations by allowing easy access to elements deep on the stack.
Similarly, `slurp` and `spew` help move bulk data between the stack and heap.

User interactions were also targeted. The included `printf` function provides a
variety of substitutions to ease user interactions. For user input, `get user
string` and `atoi` allow easy creation of basic user interfaces.

The library includes a variety of bitwise logic functions as well as heap
manipulation functions and a handful of math functions including a random
number generator.


# Instructions #

Before we can use this library, we must `#include` it in our program.  Looking
at the "Entry Points" table below, if we wanted to call `deepdup` we would need
to `#include <stack.pvvs>` in our code, but where?

Recall that VVhitespace processes our code from top to bottom. Thus, it is
always safe to `#include` files at the bottom, after our program's text. This
way the files are included in our source code but won't be accidentally
executed by the interpreter. For example:

    @ Put two elements on the stack.
    SSSTTTTSTSSN    | PUSH 244
    SSSTN           | PUSH 1
    @ Duplicate the deeper element.
    SSSTSN          | PUSH 2 (argument to deepdup)
    NSTTTSSN        | JSR > 1100 (deepdup)
    NNN             | DIE
    #include <stack.pvvs>

What about that `PUSH 2` instruction that is an argument to `deepdup`? If we
check `stack.pvvs`, we will find the following comment above the `deepdup`
function:

    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @ Name:
    @   deepdup
    @ Description:
    @   Duplicates an item deep on the stack, placing the duplicate on TOS.
    @   By default, maximum depth is 13.
    @   True maximum depth is (max depth of stackrotate & stackrotatereverse)-1.
    @ Call Stack:
    @   stack word n
    @   ...
    @   stack word 1
    @   dupdepth   <-- TOS
    @ Return Stack: (dupdepth=3)
    @   stack word n
    @   ...
    @   stack word 1
    @   copy of stack word 3   <-- TOS
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    NSSVTTSSN               | Mark: 1100 (deepdup)
    ...

From the "Call Stack" example, we can see that `deepdup` requires a `dupdepth`
argument on the TOS. Since we wanted to duplicate the second item on the stack,
we used `PUSH 2` immediately before calling `deepdup`.

In addition to the call stack, the code comments also show you what to expect
on the return stack, as well as any other information you might need in order
to use the function.


# Resource Reservations #


## Entry Points ##

The following labels are entry points to stdlib functions. Read the
header comment for each function to learn the call and return stack.

         000xxx - reserved
         001xxx - core functions
           1000 ----- printf                        (stdio.pvvs)
           1001 ----- print number from stack       (stdio.pvvs)
           1010 ----- stackrotate                   (stack.pvvs)
           1011 ----- stackrotatereverse            (stack.pvvs)
           1100 ----- deepdup                       (stack.pvvs)
         010xxx - math functions
          10000 ----- random                        (math.pvvs)
          10001 ----- absolute value                (math.pvvs)
          10010 ----- greatest common divisor       (math.pvvs)
          10011 ----- fastrand                      (math.pvvs)
         011xxx - heap functions
          11000 ----- memset                        (heap.pvvs)
          11001 ----- memcpy                        (heap.pvvs)
          11010 ----- memrand                       (heap.pvvs)
          11011 ----- memcmp                        (heap.pvvs)
          11100 ----- memsrch                       (heap.pvvs)
          11101 ----- <empty>
          11110 ----- slurp                         (heap.pvvs)
          11111 ----- spew                          (heap.pvvs)
         100xxx - string functions
         100000 ----- strlen                        (string.pvvs)
         100001 ----- isdigit                       (string.pvvs)
         100010 ----- get user string               (string.pvvs)
         101xxx - logic functions
         101000 ----- not                           (logic.pvvs)
         101001 ----- and                           (logic.pvvs)
         101010 ----- or                            (logic.pvvs)
         101011 ----- xor                           (logic.pvvs)
         101100 ----- rshift                        (logic.pvvs)
         101101 ----- lshift                        (logic.pvvs)
         110xxx - conversion functions
         110000 ----- atoi                          (convert.pvvs)
         111xxx - debug functions
         111000 ----- dump heap                     (debug.pvvs)
         111001 ----- dump stack                    (debug.pvvs)
         111010 ----- print sign                    (debug.pvvs)
         111011 ----- print magnitude               (debug.pvvs)
         111100 ----- print string                  (debug.pvvs)
         111101 ----- print signed number           (debug.pvvs)
         111110 ----- stdlib version                (debug.pvvs)
        1xxxxxx - reserved for less common entry points
        1000000 ----- lowbitand                     (logic.pvvs)
        1000001 ----- <empty>
        1000010 ----- print sign of number          (stdio.pvvs)
        1000011 ----- print magnitude of number     (stdio.pvvs)
        1000100 ----- print string from stack       (stdio.pvvs)
        1000101 ----- print string from heap        (stdio.pvvs)


## Labels ##

Since all labels share a global namespace, the standard library makes the
following reservations:

    00000000 0xxxxxxx - reserved for stdlib function entry points
    00000000 1xxxxxxx - available for use in user programs
    0xxxxxxx xxxxxxxx - reserved for private use by stdlib
    1xxxxxxx xxxxxxxx - available for use in user programs


## Heap and Pointers ##

The first 16 heap addresses (`0-15`) are reserved when using the stdlib.
Within that reservation, heap[0] is used by `random` and the block
heap[1]-heap[15] by the stack rotation subroutines which time-share
pseudo-registers between the various stdlib subroutines.

By convention, chosen since no function other than `random` should use heap[0],
functions which return a pointer will use the address `0` to represent a `NULL`
pointer.


# Misc #


## Bitwise Logic Constants ##

Be cautious when pushing constants in your code for use as bit arrays. Due to
the mismatch between the VVhitespace language's sign-magnitude representation
of integers and the interpreter's internal twos-complement representation, bit
arrays with a leading `1` (i.e. negative numbers) may appear quite different
than expected in your source code.

For example, to push a 64-bit array of all `1`'s on to the stack we must push
`SSTTN`, or `-1`.


## Extending Heap Reservation ##

By default, the stdlib uses the first 16 heap addresses. All heap access (other
than heap[0] as a seed) occurs through `stackrotate` and `stackrotatereverse`.
Edit these functions to increase the stdlib's heap reservation.

The remainder of the stdlib is written to automatically use the new allocation.
Functions like `printf`, for example, allow more substitutions when the heap
allocation is increased.


## Private Label Space ##

By convention, each public stdlib label will have 8 bits of private label space
associated with it, formed as follows:

    00001000 xxxxxxxx - for use by 1000
    00001001 xxxxxxxx - for use by 1001
    ...etc


## Strings ##

Strings in VVhitespace are stored as one character per 64-bit word since the
`LOAD` and `STORE` instructions are word length and the heap is word
addressable.

All strings terminate with an ASCII NUL (`\0`) character.


## Using the C Preprocessor ##

The standard library uses ordinary include guards of the following form:

    #ifndef FOO
    #define FOO
    ...
    #endif

This means you can `#include` a file multiple times without problems. To ease
refactoring, I recommend writing the `#include` statements per-function rather
than per-file. See the stdlib for examples.

Use of the C Preprocessor also means its syntax must be respected as though the
file were C rather than VVhitespace. For example, use of a single apostrophe in
a VVhitespace comment (e.g. don't) throws out a warning:

    warning: missing terminating ' character [-Winvalid-pp-token]

You won't run into any errors if you copy the stdlib's format but if you
stray I'm sure one could concoct some combination of characters that is both a
comment in VVhitespace and a hard error for `cpp`.


## Whitespace Compatibility ##

Most of this library will run on most Whitespace interpreters. Some parts, like
the bitwise logic functions, make assumptions about the representation of
integers in the interpreter that may be less portable than the rest of the
library.

Regardless, tests are included for every stdlib function and can be run though
the Whitespace interpreter of your choice to determine compatibility.

