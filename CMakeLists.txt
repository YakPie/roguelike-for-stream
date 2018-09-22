cmake_minimum_required (VERSION 3.3)
project(YakPie_s_Roguelike)

set(CMAKE_C_FLAGS "-std=c11 -Wall")
find_package("Curses" REQUIRED)

set(SRC
	src/components/components.c
	src/components/components.h
	src/database/core.h
	src/database/core.c
	src/database/query.c
	src/database/query.h
	src/database/vm.h
	src/database/vm.c
	src/database/repl.c
	src/database/repl.h
	src/systems/grammer_parser.c
	src/systems/grammer_parser.h
	src/systems/pcg_dungeon.h
	src/systems/pcg_dungeon.c
	src/systems/rendering_ncurses.c
	src/systems/rendering_ncurses.h
	)

add_executable(main src/main.c ${SRC})
add_executable(dbtest src/db_test.c ${SRC})
add_executable(dbquerytest src/db_query_test.c ${SRC})

target_link_libraries(main ${CURSES_LIBRARY})
target_link_libraries(dbtest ${CURSES_LIBRARY})
target_link_libraries(dbquerytest ${CURSES_LIBRARY})
