# Overview #

This document is a complete specification for the VVhitespace language.

Since Whitespace and VVhitespace are closely related, this document is heavily
copied/adapted from the "Whitespace Tutorial" published by e.c.brady@dur.ac.uk.

# Reference #

The only lexical tokens in the VVhitespace language are

  * Tab          (ASCII 9),
  * Line Feed    (ASCII 10),
  * Vertical Tab (ASCII 11), and
  * Space        (ASCII 32).

The language itself is an imperative, stack based language. Each command
consists of a series of tokens, beginning with the Instruction Modification
Parameter (IMP). These are listed in the table below.

| IMP          | Meaning            |
| :----------- | :----------------- |
| [Space]      | Stack Manipulation |
| [Tab][Space] | Arithmetic         |
| [Tab][Tab]   | Heap access        |
| [LF]         | Flow Control       |
| [Tab][LF]    | I/O                |

The virtual machine on which programs run has a stack and a heap, both of
fixed, implementation defined size and both designed around a 64-bit word.  The
programmer may push and pop words from the stack in addition to accessing the
heap on a per-word basis as a permanent store of variables and data structures.

Many commands require numbers or labels as parameters.

Numbers are integers ranging from `-(2^63)` to `+(2^63)-1` and are represented
in sign-magnitude format as a sign, either [Space] for positive or [Tab] for
negative, followed by a series of [Space] (zero) and [Tab] (one) digits
terminated by a [LF]. Note that positive and negative zero are considered
equivalent. As an example, the decimal number +42 has the following
representation.

    [Space] [Tab][Space][Tab][Space][Tab][Space] [LF]

Labels consist of an [LF] terminated list of up to sixteen spaces and tabs. The
program must not begin with a label. There is only one global namespace so all
labels must be unique. Labels are left-padded with [Space] up to sixteen
characters; the following two labels are interchangeable.

           [Tab][Space][Tab] [LF]
    [Space][Tab][Space][Tab] [LF]

## Stack Manipulation (IMP: [Space]) ##

Stack manipulation is one of the more common operations, hence the shortness of
the IMP [Space]. There are four stack instructions.

| Command     | Params | Meaning                             |
| :---------- | :----- | :---------------------------------- |
| [Space]     | Number | Push the number onto the stack      |
| [LF][Space] | ---    | Duplicate the top item on the stack |
| [LF][Tab]   | ---    | Swap the top two items on the stack |
| [LF][LF]    | ---    | Discard the top item on the stack   |

## Arithmetic (IMP: [Tab][Space]) ##

Arithmetic commands operate on the top two items on the stack, and replace them
with the result of the operation. The first item pushed is considered to be
left of the operator. The modulo command will always return a positive result.

| Command        | Params | Meaning          |
| :------------- | :----- | :--------------- |
| [Space][Space] | ---    | Addition         |
| [Space][Tab]   | ---    | Subtraction      |
| [Space][LF]    | ---    | Multiplication   |
| [Tab][Space]   | ---    | Integer Division |
| [Tab][Tab]     | ---    | Modulo           |

## Heap Access (IMP: [Tab][Tab]) ##

Heap access commands look at the stack to find the address of items to be
stored or retrieved. To store an item, push the address then the value and run
the store command. To retrieve an item, push the address and run the retrieve
command, which will place the value stored in the location at the top of the
stack.

| Command | Params | Meaning  |
| :------ | :----- | :------- |
| [Space] | ---    | Store    |
| [Tab]   | ---    | Retrieve |

## Flow Control (IMP: [LF]) ##

Flow control operations are also common. Subroutines are marked by labels, as
well as the targets of conditional and unconditional jumps, by which loops can
be implemented. Programs must be ended by means of [LF][LF][LF] so that the
interpreter can exit cleanly.

| Command              | Params | Meaning                          |
| :------------------- | :----- | :------------------------------- |
| [Space][Space][VTab] | Label  | Mark a location in the program   |
| [Space][Tab]         | Label  | Call a subroutine                |
| [Space][LF]          | Label  | Jump unconditionally to a label  |
| [Tab][Space]         | Label  | Jump to label if TOS is zero     |
| [Tab][Tab]           | Label  | Jump to label if TOS is negative |
| [Tab][LF]            | ---    | Return from subroutine           |
| [LF][LF]             | ---    | End the program                  |

## I/O (IMP: [Tab][LF]) ##

Finally, we need to be able to interact with the user. There are IO
instructions for reading and writing numbers and individual characters.

The read instructions take the heap address in which to store the result from
the top of the stack.

| Command        | Params | Meaning                                           |
| :------------- | :----- | :------------------------------------------------ |
| [Space][Space] | ---    | Output the character at the TOS                   |
| [Space][Tab]   | ---    | Output the number at the TOS                      |
| [Tab][Space]   | ---    | Read character and store at location given by TOS |
| [Tab][Tab]     | ---    | Read number and store at location given by TOS    |
