#ifndef RENDERING_NCURSES_H 
#define RENDERING_NCURSES_H 

#include "../components/components.h"

struct Point get_offset() {
	int row, col;
	getmaxyx(stdscr, row, col);

	struct Point ret = {
		.x = col / 2,
		.y = row - 15
	};

	return ret;
}

void print_room_ncurses(struct Graph* dag) {
	struct Point offset = get_offset();

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

		mvprintw(y + offset.y, x + offset.x, "%c", room_type);
	}

	for(int i=0; i < dag->number_of_edges; i++) {
		int x = dag->edges[i].from.x + dag->edges[i].to.x,
			 y = dag->edges[i].from.y + dag->edges[i].to.y;

		mvprintw(y + offset.y, x + offset.x, ".");
	}

	refresh();
}



#endif
