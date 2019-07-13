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

         000xxx - reserved
         001xxx - print functions
           1000 ----- print string from stack       (stdio.pvvs)
           1001 ----- print string from heap        (stdio.pvvs)
           1010 ----- print number from stack       (stdio.pvvs)
         010xxx - math functions
          10000 ----- random                        (math.pvvs)
          10001 ----- absolute value                (math.pvvs)
         011xxx - heap functions
          11000 ----- memset                        (heap.pvvs)
         100xxx - unassigned
         101xxx - unassigned
         110xxx - conversion functions
         111xxx - debug functions
         111000 ----- dump heap                     (debug.pvvs)
        1xxxxxx - reserved for less common entry points
        1000000 ----- print sign of number          (stdio.pvvs)
        1000001 ----- print magnitude of number     (stdio.pvvs)

# Misc #

By convention, each public stdlib label will have 8 bits of
private label space associated with it, formed as follows:

    00001000 xxxxxxxx - for use by 1000
    00001001 xxxxxxxx - for use by 1001
    ...etc
