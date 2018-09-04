all: src/main

src/main: src/main.c
	@mkdir -p build/
	gcc -ggdb -g src/main.c -lncurses -Ibuild -o ./build/main

run: src/main
	./build/main level1 

clean:
	@rm ./build/*
