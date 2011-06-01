all: fdtool

fdtool: fdtool.c Makefile
	gcc fdtool.c -o fdtool

test: fdtool
	./fdtool --pipe 7 8 -- ./test.sh --somearg

.PHONY: test
