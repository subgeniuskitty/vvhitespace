# Overview #

This folder contains tests for the VVhitespace standard library ('stdlib').


# Instructions #

Edit the shebang in `vv_test.py` to match your environment. For example:

    FreeBSD 12: #!/usr/local/bin/python3.6
    Debian 9  : #!/usr/bin/python3

Build `vvc` and `vvi` in the source tree, if you haven't already.

    vvs-repo/stdlib_tests % cd .. && make clean all && cd stdlib_tests
    cc -Wall -std=c99 -o vvc vv_compiler.c
    cc -Wall -std=c99 -o vvi vv_interpreter.c
    vvs-repo/stdlib_tests %

Alternatively, edit the configuration block in `vv_test.py` to provide
appropriate paths relative to this `stdlib_tests` folder.

    compiler_path = '../vvc'
    interpreter_path = '../vvi'
    include_path = '-I../stdlib'

With configuration now complete, execute the tests via `make test`.  A dot will
appear for every successfully completed test. For example:

    vvs-repo/stdlib_tests % make test
    Testing stdlib:
    ..................................
    vvs-repo/stdlib_tests %

If a test should fail, the name of the test will be printed in place of its
dot.  For example, by breaking the `and` subroutine, the corresponding test
fails for `and` and for all subroutines which depend on `and`.

    vvs-repo/stdlib_tests % make test
    Testing stdlib:
    ................
    3004_and
            Expected: +0+0+1+1+42
            Received: +3+3+4+4+45
    
    3005_or
            Expected: +0+1+1-1-1
            Received: -3-2-2-4-4
    
    3006_xor
            Expected: +0+1+0-2-1
            Received: -1-1-3-5-1
    ...............
    vvs-repo/stdlib_tests %

If testing is aborted prematurely, say by a Ctrl-C initiated SIGINT, use `make
clean` to remove any temporary files.


# Add New Test #

Most tests consist of four basic parts:

 1. Perform a test.
 2. Print a result.
 3. Die.
 4. Dependencies

With that framework in mind, consider `0001_dumpstack.pvvs`, the first test,
with line numbers for reference.

    1:  SSSTTTTTTTTN    | PUSH 255
    2:  SSSTSTSTSN      | PUSH 42
    3:  SSSTSN          | PUSH 2 (count)
    4:  NSTTTTSSTN      | JSR > 111001 (dumpstack)
    5:  NNN             | DIE
    6:  
    7:  #include <debug.pvvs>

Comparing this to the four part framework, we see that lines 1-4 perform the
test and print the result, combined in this case since our test function's goal
is to print something. Line 5 then terminates the program, after which the
dependencies are included on line 7.

Once you have written a test of this form, add it to the `tests` array in the
file `vv_test.py`. The three fields are:

    ['filename_without_extension', 'string for stdin', 'string for expected stdout']

Continuing with the same example:

    ['0001_dumpstack', '', 'TOS:\n2:\t+42\n1:\t+255\n'],

Note that the first field was the name of the file in which our test was saved,
minus the extension.  These filenames should be numbered, grouped by category
and ordered by dependency.

The second field is empty since our test doesn't require any simulated input
from the user.

The final field is the expected output from our test. In this example, with
tabs and newlines expanded:

    TOS:
    2:      +42
    1:      +255

Each time the test is executed by `vv_test.py` it will be fed the input from
the second field and the output will be compared against the third field. If
there is any mismatch, the test has failed and the user will be alerted with an
error.

