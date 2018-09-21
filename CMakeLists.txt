cmake_minimum_required (VERSION 3.3)
project(YakPie_s_Roguelike)

find_package("Curses" REQUIRED)

set(SRC
	src/components/components.c
	src/components/components.h
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
