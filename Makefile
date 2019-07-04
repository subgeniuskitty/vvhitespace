# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

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

test: all
	@echo "Starting test suite:"
	@./vv_test.py

clean:
	@rm -f vvc vvc.core vvi vvi.core
