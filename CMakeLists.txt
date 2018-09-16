cmake_minimum_required (VERSION 3.3)
project(YakPie_s_Roguelike)

find_package("Curses" REQUIRED)

add_executable(main src/main.c)
add_executable(dbtest src/db_test.c)
add_executable(dbquerytest src/db_query_test.c)

target_include_directories(main PUBLIC )
target_link_libraries(main ${CURSES_LIBRARY})
