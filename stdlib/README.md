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

## Heap and Pointers ##

The first 16 heap addresses (`0-15`) are reserved when using the stdlib.
Within that reservation, heap[0] is used by `random` and the block
heap[2]-heap[15] by the stack rotation subroutines which time-share these
pseudo-registers between the various stdlib subroutines.

By convention, functions which return a pointer will use the address `0` to
represent a `NULL` pointer.

# Entry Points #

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
         101xxx - logic functions
         101000 ----- not                           (logic.pvvs)
         101001 ----- and                           (logic.pvvs)
         101010 ----- or                            (logic.pvvs)
         101011 ----- xor                           (logic.pvvs)
         101100 ----- rshift                        (logic.pvvs)
         101101 ----- lshift                        (logic.pvvs)
         110xxx - conversion functions
         111xxx - debug functions
         111000 ----- dump heap                     (debug.pvvs)
         111001 ----- dump stack                    (debug.pvvs)
        1xxxxxx - reserved for less common entry points
        1000000 ----- lowbitand                     (logic.pvvs)
        1000001 ----- isnegative                    (math.pvvs)
        1000010 ----- print sign of number          (stdio.pvvs)
        1000011 ----- print magnitude of number     (stdio.pvvs)
        1000100 ----- print string from stack       (stdio.pvvs)
        1000101 ----- print string from heap        (stdio.pvvs)

# Misc #

By convention, each public stdlib label will have 8 bits of
private label space associated with it, formed as follows:

    00001000 xxxxxxxx - for use by 1000
    00001001 xxxxxxxx - for use by 1001
    ...etc
