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

//	char* starting_rules = "sfpllffpfforrrorrffe";
	char* starting_rules = "se";
	int num_replacements = 10;

	// Setup ncurses
	initscr();
//	setlocale(LC_ALL,"C-UTF-8");
	noecho();

	int ch;
	char* output = NULL;
	struct Graph* dag = NULL;

	const int KEY_D = 100;
	const int KEY_W = 119;
	const int KEY_A = 97;
	const int KEY_S = 115;
	const int KEY_ENTER = 10;

	int x = 0,
		 y = 0;

	while(true) {
		// Prints some debug info
		mvprintw(1, 10, "Dungeon");
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
		print_room_ncurses(dag);
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
			default:
				mvprintw(5, 10, "%d", ch);
		}
		
	}
	
	free(output);
	free(dag);

	endwin();

	return 0;
}
