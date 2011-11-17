compile:
	colorgcc -pg ./src/main.c ./src/core/*.c -combine -o ./bin/main -Wall -lm -Isrc/include
run: compile
	./bin/main

