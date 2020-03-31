# (c) 2019 Aaron Taylor <ataylor at subgeniuskitty dot com>
# See LICENSE.txt file for copyright and license details.

EXAMPLES != find . -mindepth 1 -maxdepth 1 -type d

all:
	@echo "No target available to build all."
	@echo "Try 'make run' inside an example folder."

clean:
	@for dir in $(EXAMPLES); do \
		echo Cleaning $$dir; \
		(cd $$dir; ${MAKE} clean); \
	done
