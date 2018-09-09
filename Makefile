all: src/main

src/main: src/main.c
	@mkdir -p build/
	gcc -ggdb -g src/main.c -lncurses -Ibuild -o ./build/main

src/db_test: src/db_test.c
	@mkdir -p build/
	gcc -ggdb -g src/db_test.c -lncurses -Ibuild -o ./build/db_test

run: src/main
	./build/main level1 

run_db_test: src/db_test
	./build/db_test

clean:
	@rm ./build/*
