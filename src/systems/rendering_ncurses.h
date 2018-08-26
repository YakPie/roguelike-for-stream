#ifndef RENDERING_NCURSES_H 
#define RENDERING_NCURSES_H 

#include "../components/components.h"

void print_room_ncurses(struct Graph* dag) {
	int row, col;
	getmaxyx(stdscr, row, col);

	int offset_y = row - 15;
	int offset_x = col / 2;

	for(int i=0; i < dag->number_of_nodes; i++) {
		char room_type = dag->nodes[i].type; 
		switch(dag->nodes[i].type) {
			case 's':
				room_type = 'S';
				break;
			case 'e':
				room_type = 'E';
				break;
			default:
				room_type = 'R';
				break;
		}

		int x = dag->nodes[i].position.x * 2,
			 y = dag->nodes[i].position.y * 2;

		mvprintw(y + offset_y, x + offset_x, "%c", room_type);
	}

	for(int i=0; i < dag->number_of_edges; i++) {
		int x = dag->edges[i].from.x + dag->edges[i].to.x,
			 y = dag->edges[i].from.y + dag->edges[i].to.y;

		mvprintw(y + offset_y, x + offset_x, ".");
	}

	refresh();
}



#endif
