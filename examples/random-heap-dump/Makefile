# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

include ../config.mk

all: randomheap

randomheap:
	$(CPP) $(CPP_FLAGS) -o temp.pvvs randomheap.pvvs
	$(VVS_COMPILER) -i temp.pvvs -o randomheap.vvs
	@rm -f temp.pvvs

run: randomheap
	$(VVS_INTERPRETER) -i randomheap.vvs

clean:
	@rm -f randomheap.vvs temp.pvvs
