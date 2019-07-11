# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

include ../config.mk

all: hello

hello:
	$(VVS_COMPILER) -i hello.pvvs -o hello.vvs

run:
	$(VVS_INTERPRETER) -i hello.vvs

clean:
	@rm -f hello.vvs
