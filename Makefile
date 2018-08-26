all: src/main

src/main: src/main.c
	gcc -g src/main.c -lncurses -o ./main

clean:
	@rm ./main
