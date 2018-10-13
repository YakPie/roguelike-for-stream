cmake_minimum_required (VERSION 3.3)
project(YakPie_s_Roguelike)

set(CMAKE_C_FLAGS
	"-std=c11 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wconversion \
	-Wcast-align \
	-Wcast-qual \
	-Wpointer-arith \
	-Wshadow"
)

find_package("Curses" REQUIRED)

add_library(game
	src/entity/entity.h
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
	src/database/internal.h
	src/database/internal.c
	src/systems/grammer_parser.c
	src/systems/grammer_parser.h
	src/systems/pcg_dungeon.h
	src/systems/pcg_dungeon.c
	src/systems/rendering_ncurses.c
	src/systems/rendering_ncurses.h
	src/systems/systems.h
	src/systems/systems.c
	src/systems/dummy_system.h
	src/systems/dummy_system.c
	src/systems/frame_counter.h
	src/systems/frame_counter.c
	src/systems/repl_subsystem.h
	src/systems/repl_subsystem.c
	)

add_executable(main src/main.c)
add_executable(dbtest src/db_test.c)
add_executable(dbquerytest src/db_query_test.c)

target_link_libraries(main ${CURSES_LIBRARY} game)
target_link_libraries(dbtest ${CURSES_LIBRARY} game)
target_link_libraries(dbquerytest ${CURSES_LIBRARY} game)
