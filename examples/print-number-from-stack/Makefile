# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

include ../config.mk

all: printnum

printnum:
	$(CPP) $(CPP_FLAGS) -o temp.pvvs printnum.pvvs
	$(VVS_COMPILER) -i temp.pvvs -o printnum.vvs
	@rm -f temp.pvvs

run: printnum
	$(VVS_INTERPRETER) -i printnum.vvs

clean:
	@rm -f printnum.vvs temp.pvvs
