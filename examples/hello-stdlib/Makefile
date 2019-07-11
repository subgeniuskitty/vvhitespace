# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

include ../config.mk

all: hello

hello:
	$(CPP) $(CPP_FLAGS) -o temp.pvvs hello.pvvs
	$(VVS_COMPILER) -i temp.pvvs -o hello.vvs
	@rm -f temp.pvvs

run: hello
	$(VVS_INTERPRETER) -i hello.vvs

clean:
	@rm -f hello.vvs temp.pvvs
