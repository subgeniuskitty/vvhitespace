VVhitespace is descended from Whitespace, adding a vertical tab to the language
along with some restrictions to ease implementation. The name is intended to
embrace the spirit of Whitespace's visual incomprehensibility.

Since Whitespace ignores [VTab] as a comment character, and since the
Whitespace VM is a superset of the VVhitespace VM, all valid VVhitespace
programs are also valid Whitespace programs.

--------------------------------------------------------------------------------

TODO: Finish this up based on what I've found below.

It seems that all (most?) WS interpreters locate labels using one of two methods.

  * **Method 1** Scan from the start of the file for the first occurence of the
    mark-label bytestring and jump.

  * **Method 2** Scan from the start of the file, looking for a mark-label
    bytestring, but 'parsing' one bytestring at a time, and jumping to the
    first 'standalone' mark-label bytestring. Note that this is different than
    executing the program, particularly when user-input commands are present.

Both of these methods can be broken:

  * Type 1: No 'standalone' label exists. This breaks Method 2.

    This should print a '!' before infinite '.' lines.

  * Type 2: Hidden label before 'standalone' label. This breaks Method 1.

    This should print an infinite chain of '.' lines.

This is the Type 1 program:

    SSSTSSSSTN  | Push +33 (ASCII !)
    NSNSTSTTTSN | JMP>label0
    NSSTTTTN    | MARK label2
    SSSSN       | PUSH +0
    SNN         | DROP
    SSSTSTTTSN  | Push +46 (ASCII .)
    TNSS        | Output character
    SSSTSTSN    | Push +10 (ASCII newline)
    TNSS        | Output character
    NSNTTTTN    | JMP>label2

Append this to turn it into the Type 2 program:

    NSSSTSTTTSN | MARK label0 (2nd time)
    NSNTTTTN    | JMP>label2

WS Interpreter Evaluations:

    Method 1:
        whitespacers/c: (c) meth0dz

    Method 2:
        whitespacers/ruby: (c) 2003 by Wayne E. Conrad
        whitespacers/perl: (c) 2003 Micheal Koelbl
        whitespacers/haskell: (c) 2003 Edwin Brady
        threeifbywhiskey/satan
