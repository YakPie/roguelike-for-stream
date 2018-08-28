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

void print_room_info(struct Graph* dag, int x, int y) {
	struct Point offset = get_offset();

	for(int i=0; i<dag->number_of_nodes; i++) {
		if(dag->nodes[i].position.x == x &&
			dag->nodes[i].position.y == y)
		{
			mvprintw(7, 10, "Room debug:");
			int monsters_n = dag->nodes[i].number_of_monsters;
			for(int j=0; j<monsters_n; j++) {
				switch(dag->nodes[i].monsters[j].type) {
					case MONSTER_SLIME:
						mvprintw(8 + j, 10, " - Slime");
						break;
					case MONSTER_GOBLIN:
						mvprintw(8 + j, 10, " - Goblin");
						break;
				}
			}
			break;
		}
	}

	//const wchar_t ghost_emoji = 0x1F47B;
	//mvprintw(y * 2 + offset.y, x * 2 + offset.x, "%lc", ghost_emoji);
	mvprintw(y * 2 + offset.y, x * 2 + offset.x, "@");
}

int main(void)
{
	srand(time(NULL));

	/*
	 *  s = start of the level / entry
	 *  e = end of the level
	 *  l = moved left and create a room
	 *  r = move right and create a room
	 *  f = move upward/forward and create a room
	 *  p = push rules (pushes the current position to the stack)
	 *  o = pop rules (pops position of the stack)
	 *  q = turn clockwise
	 *  a = turn anti-clockwise
	 *  0 = reset direction
	 *  1 = slime
	 *  2 = goblin
	 */
	struct Rules rules[] = {
//		{ .id = 'f',  .replace = "s" },
		{ .id = 's',  .replace = "sl" },
		{ .id = 's',  .replace = "sr" },
		{ .id = 's',  .replace = "sf" },
		{ .id = 'l',  .replace = "l1" },
		{ .id = 'l',  .replace = "l" },
		{ .id = 'r',  .replace = "r2" },
		{ .id = 'r',  .replace = "r" },
//		{ .id = 'l',  .replace = "ll" },
//		{ .id = 'l',  .replace = "l" },
//		{ .id = 'l',  .replace = "llplfffrror" },
		{ .id = 's',  .replace = "spllffffrro" },
//		{ .id = 's',  .replace = "sprrrrrro" },
		{ .id = 's',  .replace = "safqfqf" },
	};
//	char* starting_rules = "sfpllffpfforrrorrffe";
	char* starting_rules = "se";
	int num_replacements = 10;
	const int rules_length = sizeof(rules)/sizeof(*rules);

	// Setup ncurses
	initscr();
	setlocale(LC_ALL,"C-UTF-8");
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
		
		if(output == NULL || dag == NULL || ch == KEY_ENTER) {
			if(output != NULL) free(output);
			if(dag != NULL) free(dag);

			output = rule_engine(
				rules,
				starting_rules,
				num_replacements,
				rules_length
			); //*/

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
