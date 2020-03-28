# Overview #

This folder contains tests for the VVhitespace interpreter (`vvi`).

# Instructions #

Edit the shebang in `vv_test.py` to match your environment. For example:

    FreeBSD 12: #!/usr/local/bin/python3.6
    Debian 9  : #!/usr/bin/python3

Build `vvc` and `vvi` in the source tree, if you haven't already.

    vvs-repo/tests % cd .. && make clean all && cd tests

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
