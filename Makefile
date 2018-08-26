all: src/main

src/main: src/main.c
	@mkdir -p build/
	gcc -g src/main.c -lncurses -Ibuild -o ./build/main

clean:
	@rm ./build/*
