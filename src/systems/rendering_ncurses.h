#ifndef RENDERING_NCURSES_H 
#define RENDERING_NCURSES_H 

#include "../components/components.h"

struct Point get_offset();

void print_room_ncurses(struct Graph* dag, int x, int y);
void print_debug_room_info(struct Graph* dag, int x, int y);


#endif
