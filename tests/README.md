# Overview #

This folder contains tests for the VVhitespace interpreter (`vvi`).


# Instructions #

Edit the shebang in `vv_test.py` to match your environment. For example:

    FreeBSD 12: #!/usr/local/bin/python3.6
    Debian 9  : #!/usr/bin/python3

Build `vvc` and `vvi` in the source tree, if you haven't already.

    vvs-repo/tests % cd .. && make clean all && cd tests
    cc -Wall -std=c99 -o vvc vv_compiler.c
    cc -Wall -std=c99 -o vvi vv_interpreter.c
    vvs-repo/tests %

Alternatively, edit the configuration block in `vv_test.py` to provide
appropriate paths relative to this `tests` folder. 

    compiler_path = '../vvc'
    interpreter_path = '../vvi'

With configuration now complete, execute the tests via `make test`.  A dot will
appear for every successfully completed test. For example:

    vvs-repo/tests % make test
    Testing vvi:
    .......................
    vvs-repo/tests %

If a test should fail, the name of the test will be printed in place of its
dot.  For example, with an induced failure in the division command:

    vvs-repo/tests % make test
    Testing vvi:
    ........
    2004_arithmetic_division
        Expected: A
        Received: A113
    ..............
    vvs-repo/tests %

If testing is aborted prematurely, say by a Ctrl-C initiated SIGINT, use `make
clean` to remove any temporary files.


# Add New Test #

Consider the test `5003_io_input_character.pvvs` as an example, reproduced
below with line numbers.

    1:  SSSTN           | ST: Push +1
    2:  TNTS            | IO: Input character
    3:  SSSTN           | ST: Push +1
    4:  TTT             | HP: Load
    5:  TNSS            | IO: Output character
    6:  NNN             | FC: Terminate program

With the goal of testing the `get character from user` instruction, the test
begins on lines 1-2 by setting up for and then using this instruction. Lines
3-5 print the result and line 6 terminates the test.

Once you have written a test of this form, add it to the `tests` array in the
file `vv_test.py`. The three fields are:

    ['filename_without_extension', 'string for stdin', 'string for expected stdout']

Continuing the same example:

    ['5003_io_input_character', 'A', 'A'],

Note that the first field was the name of the file in which our test was saved,
minus the extension.  These filenames should be numbered, grouped by category
and ordered by dependency.

The second field is a string from which our test program may read. Since this
is a test of reading an input character, the string is populated with `A`.

The final field is the expected output from our test. In this example, this is
simply the same character that was read from the previous field, `A`. 

Each time the test is executed by `vv_test.py` it will be fed the input from
the second field and the output will be compared against the third field. If
there is any mismatch, the test has failed and the user will be alerted with an
error.

