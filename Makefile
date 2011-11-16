compile:
	colorgcc ./src/main.c ./src/core/*.c -o ./bin/main -Wall -lm -Isrc/include
run: compile
	./bin/main
test: ctest
	./bin/test
ctest:
	colorgcc ./src/test/test.c ./src/core/*.c -o ./bin/test -Wall -lm -Isrc/include
