# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# See LICENSE.txt file for copyright and license details.

####################################################################################################
# Configuration

CC              = cc
CC_FLAGS        = -Wall -std=c99

####################################################################################################
# Build

all: vvc vvi

vvi:
	$(CC) $(CC_FLAGS) -o $@ vv_interpreter.c

vvc:
	$(CC) $(CC_FLAGS) -o $@ vv_compiler.c

clean:
	@rm -f vvc vvc.core vvi vvi.core

test: testvvi teststdlib

testvvi: all
	@cd tests; make test

teststdlib: all
	@cd stdlib_tests; make test

testclean:
	@cd tests; make clean
	@cd stdlib_tests; make clean
