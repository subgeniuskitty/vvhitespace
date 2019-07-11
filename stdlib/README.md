# Overview #

This folder contains a library of useful functions written in VVhitespace.
Standard include guards are used with `cpp` to include the stdlib in user
programs. For an example, see `examples/hello-stdlib`.

# Reservations #

Since all labels share a global namespace, the standard library makes the
following reservations:

## Label ##

    00000000 0xxxxxxx - reserved for stdlib function entry points
    00000000 1xxxxxxx - unassigned
    0xxxxxxx xxxxxxxx - reserved for private use by stdlib
    1xxxxxxx xxxxxxxx - available for use in user programs

## Heap ##

    The first 256 heap addresses are reserved when using the stdlib.

# Entry Points #

The following labels are entry points to stdlib functions. Read the
header comment for each function to learn the call and return stack.

    stdio.pvvs:
           1000 - print string from stack
           1001 - print string from heap

# Misc #

By convention, each public stdlib label will have 8 bits of
private label space associated with it, formed as follows:

    00001000 xxxxxxxx - for use by 1000
    00001001 xxxxxxxx - for use by 1001
    ...etc
