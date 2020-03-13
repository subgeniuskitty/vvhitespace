While considering methods for further obfuscating Whitespace code, I realized
that the Whitespace language reference didn't enforce any particular alignment
or grouping for code execution. In theory this means one could create a
sequence of Whitespace trits which represents different sequences of commands
depending on exactly where execution begins.

One implementation of this idea is the creation of a 'hidden' label. For
example, the command sequence

    PUSH 0; DROP; PUSH 46

assembles as

    SSSSN SNN SSSTSTTTSN

which can be visually regrouped as

    SSSSNSN NSSSTSTTTSN

and contains the `MARK label0` command (i.e. `NSSSTSTTTSN`) used in the next
set of examples.

Additionally, since `PUSH 0; DROP` is effectively a `NOP`, 'hijacking' the code
at this location allows one to insert their own integer on the stack in place
of the `PUSH 46` command, sneakily substituting it as an input to any
downstream processing.

I decided to investigate the behavior of specific Whitespace interpreters,
discovering that they broke down into two methods for locating labels.

  * **Method 1** Scan from the start of the file for the first occurence of the
    mark-label bytestring and jump.
    
    Examples:
    
        whitespacers/c: (c) meth0dz

  * **Method 2** Scan from the start of the file, looking for a mark-label
    bytestring, but 'parsing' one bytestring at a time, and jumping to the
    first 'standalone' mark-label bytestring. Note that this is different than
    executing the program, particularly when user-input commands are present.
    
    Examples:
    
        whitespacers/haskell: (c) 2003 Edwin Brady
        whitespacers/ruby: (c) 2003 by Wayne E. Conrad
        whitespacers/perl: (c) 2003 Micheal Koelbl
        threeifbywhiskey/satan

Both of these methods can be broken using valid Whitespace code:

  * **Type A**: No 'standalone' label exists. This breaks Method 2.

    By programmer's intent, this should print a '!' before infinite '.' lines.

  * **Type B**: Hidden label before 'standalone' label. This breaks Method 1.

    By programmer's intent, this should print an infinite chain of '.' lines.

This is the Type A program:

    SSSTSSSSTN  | Push +33 (ASCII '!')
    NSNSTSTTTSN | JMP > 0101110 (label0)
    NSSTTTTN    | MARK: 1111 (label2)
    SSSSN       | PUSH 0
    SNN         | DROP
    SSSTSTTTSN  | Push +46 (ASCII '.')
    TNSS        | Output character
    SSSTSTSN    | Push +10 (ASCII '\n')
    TNSS        | Output character
    NSNTTTTN    | JMP > 1111 (label2)

Append this to turn it into the Type B program:

    NSSSTSTTTSN | MARK: 0101110 (label0) (2nd time)
    NSNTTTTN    | JMP > 1111 (label2)

VVhitespace avoids this ambiguity by marking label definitions with a vertical
tab `[VTab]` immediately before the label.

    Old label: NSS  TSTS N
    New label: NSSV TSTS N

Since Whitespace ignores [VTab] as a comment character, and since the
Whitespace VM is a superset of the VVhitespace VM, all valid VVhitespace
programs are also valid Whitespace programs, though the task of locating a
suitable Whitespace interpreter is left for the reader.

--------------------------------------------------------------------------------

Quoting from the original Whitespace tutorial which I used as language reference:

    The programmer is free to push arbitrary width integers onto the stack.

I have yet to find a Whitespace interpreter which successfully implements that
statement.

Since I wanted to implement bitwise logic functions, this broad definition
posed a problem. For example, how many `1`s should be in the output of the
expression `NOT(0)`? Should the expression `NOT(0) == NOT(0)` always be true?

VVhitespace sidesteps the problem by declaring all integers to be 64-bits wide.

