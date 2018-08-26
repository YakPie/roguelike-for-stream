#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "components/components.h"
#include "systems/pcg_dungeon.h"
#include "systems/rendering_ncurses.h"

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
	 */
	struct Rules rules[] = {
//		{ .id = 'f',  .replace = "s" },
		{ .id = 's',  .replace = "sl" },
		{ .id = 's',  .replace = "sr" },
		{ .id = 's',  .replace = "sf" },
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

	while(true) {
		char* output = rule_engine(
			rules,
			starting_rules,
			num_replacements,
			rules_length
		);

		// Setup ncurses
		initscr();

		// Prints some debug info
		mvprintw(1, 10, "Dungeon overview:");
		mvprintw(2, 10, "Hit a random key to get a new dungeon");
		mvprintw(3, 10, "Dungeon rule:");
		mvprintw(3, 25, output);

		// Travel the dungeon rule and gennerates a DAG
		struct Graph* dag = create_dag_from_dungeonrule(output);
		print_room_ncurses(dag);

		// Wait on character
		getch();
		clear();
		endwin();

		free(output);
		free(dag);
	}

	return 0;
}
