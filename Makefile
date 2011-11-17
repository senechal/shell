compile:
	colorgcc -pg ./src/main.c ./src/core/*.c -combine -o ./bin/main -Wall -lm -Isrc/include
run: compile
	./bin/main
test: ctest
	./bin/test
ctest:
	colorgcc -pg ./src/test/test.c ./src/core/*.c -combine -o ./bin/test -Wall -lm -Isrc/include
