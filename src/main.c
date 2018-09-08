#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>
#undef KEY_ENTER

#include "components/components.h"
#include "systems/pcg_dungeon.h"
#include "systems/rendering_ncurses.h"
#include "systems/grammer_parser.h"

void print_door(
	struct Point current, 
	struct Point towards,
	struct Point offset,
	int room_width,
	int room_height
) {
	int x=0, y=0;
	// upwards
	if(towards.y < current.y && towards.x == current.x) {
		y = room_height;
		x = room_width / 2;
	}
	// downwards
	else if(towards.y > current.y && towards.x == current.x) {
		y = 0;
		x = room_width / 2;
	}
	// left
	else if(towards.y == current.y && towards.x < current.x) {
		y = room_height / 2;
		x = 0;
	}
	// right
	else if(towards.y == current.y && towards.x > current.y) {
		y = room_height / 2;
		x = room_width;
	}
	else
		return;

	mvprintw(-y + offset.y, x + offset.x, "D");
}

void print_current_room_ncurses(struct Graph* dag, int x, int y) {
	struct Point offset = get_offset();

	int row, col;
	getmaxyx(stdscr, row, col);

	// Print Walls
	int room_width = col / 2,
		 room_height = row / 2;

	offset.x -= room_width / 2;
	offset.y += 5;

	for(int x = 0; x <= room_width; x++) {
		for(int y = 0; y <= room_height; y++) {
			if(x == 0 || y == 0 || x == room_width || y == room_height)
				mvprintw(-y + offset.y, x + offset.x, "W");
		}
	}

	for(int i=0; i<dag->number_of_edges; i++) {
		if(dag->edges[i].from.x == x &&
			dag->edges[i].from.y == y)
		{
			print_door(
				dag->edges[i].from,
				dag->edges[i].to,
				offset,
				room_width,
				room_height
			);
		}

		if(dag->edges[i].to.x == x &&
			dag->edges[i].to.y == y)
		{
			print_door(
				dag->edges[i].to,
				dag->edges[i].from,
				offset,
				room_width,
				room_height
			);
		}
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));

	if(argc < 2) {
		printf("takes a level\n");
		exit(1);
	}

	FILE *fd = fopen(argv[1], "r");
	struct RulesWrapper rw = parser(fd);
	fclose(fd);

	char* starting_rules = "se";
	if(argc > 2) {
		starting_rules = argv[2];	
	}

	int num_replacements = 10;
	if(argc > 3) {
		num_replacements = atoi(argv[3]);
	}

	// Setup ncurses
	initscr();
	noecho();
	halfdelay(1);

	int ch;
	char* output = NULL;
	struct Graph* dag = NULL;

	const int KEY_D = 100;
	const int KEY_W = 119;
	const int KEY_A = 97;
	const int KEY_S = 115;
	const int KEY_M = 109;
	const int KEY_ENTER = 10;

	int x = 0,
		 y = 0;

	int tmp = 0;
	int show_map = 1;

	while(true) {
		tmp++;
		// Prints some debug info
		mvprintw(1, 10, "Dungeon %d", tmp);
		mvprintw(3, 10, "Debug rule:");
		
		mvprintw(5, 40, "id replace\n");
		for(int i=0; i < rw.number_of_rules; i++) {
			mvprintw(6+i, 40, "%c  %s\n", rw.rules[i].id, rw.rules[i].replace);
		}

		if(output == NULL || dag == NULL || ch == KEY_ENTER) {
			if(output != NULL) free(output);
			if(dag != NULL) free(dag);

			output = rule_engine(
				rw.rules,
				starting_rules,
				num_replacements,
				rw.number_of_rules
			); 

			// Travel the dungeon rule and gennerates a DAG
			dag = create_dag_from_dungeonrule(output);

			for(int i=0; i<dag->number_of_nodes; i++) {
				if(dag->nodes[i].type == 's') {
					x = dag->nodes[i].position.x;
					y = dag->nodes[i].position.y;
					break;
				}
			}
		}
		mvprintw(2, 10, "The current position is x %d and y %d", x, y);

		mvprintw(3, 25, output);
		if(show_map)
			print_room_ncurses(dag);
		else
			print_current_room_ncurses(dag, x, y);
		print_room_info(dag, x, y);

		// Wait on character
		ch = getch();
		clear();

		switch(ch) {
			case KEY_W: 
				mvprintw(5, 10, "KEY_UP!");
				y--;
				break;
			case KEY_S: 
				mvprintw(5, 10, "KEY_DOWN!");
				y++;
				break;
			case KEY_A: 
				mvprintw(5, 10, "KEY_LEFT!");
				x--;
				break;
			case KEY_D: 
				mvprintw(5, 10, "KEY_RIGHT!");
				x++;
				break;
			case KEY_M:
				show_map = show_map==1 ? 0 : 1;
			default:
				mvprintw(5, 10, "%d", ch);
		}
		
	}
	
	free(output);
	free(dag);

	endwin();

	return 0;
}
