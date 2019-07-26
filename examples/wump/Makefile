# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# All rights reserved.

include ../config.mk

all: wump

wump:
	$(CPP) $(CPP_FLAGS) -o temp.pvvs wump_strings.pvvs
	$(VVS_COMPILER) -i temp.pvvs -o wump.vvs
	@rm -f temp.pvvs

run: wump
	$(VVS_INTERPRETER) -i wump.vvs

clean:
	@rm -f wump.vvs temp.pvvs
