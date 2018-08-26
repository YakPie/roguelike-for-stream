#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "components/components.h"
#include "systems/pcg_dungeon.h"

int main(void)
{
	srand(time(NULL));

	/*
	 *  s = start of the level / entry
	 *  e = end of the level
	 *  l = moved left and create a room
	 *  r = move right and create a room
	 *  f = move upward/forward and create a room
	 *  b = backtrack (maybe in the future?)
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
		int row, col;
		getmaxyx(stdscr, row, col);
		mvprintw(1, 10, "Dungeon overview:");
		mvprintw(2, 10, "Hit a random key to get a new dungeon");
		mvprintw(3, 10, "Dungeon rule:");
		mvprintw(3, 25, output);

		struct Point pos = {
			.x = (row - 5) / 2,
			.y = col / 4
		};

		struct Point tmp_pos = pos;
		struct LinkedList* stack = malloc(sizeof(struct LinkedList));
		stack->point = tmp_pos;
		stack->next = NULL; 

		int direction = 0;

		for(int i=0; i < strlen(output); i++) {
			switch(output[i])
			{
				case 's':
					break;
				case 'l':
					pos.x--;
					break;
				case 'r':
					pos.x++;
					break;
				case 'f':
				case 'e':
					switch(direction) {
						case UP:
							pos.y--;
							break;
						case LEFT:
							pos.x--;
							break;
						case RIGHT:
							pos.x++;
							break;
						case DOWN:
							pos.y++;
							break;
					}
					break;
				case 'p': // pushing
					tmp_pos.x = pos.x;
					tmp_pos.y = pos.y;
					push(stack, tmp_pos);
					break;
				case 'o': // pop
					tmp_pos = pop(stack);
					pos.x = tmp_pos.x;
					pos.y = tmp_pos.y;
					break;
				case 'q': // clockwise
					direction = (direction+1)%4;
					break;
				case 'a': // anti-clockwise
					direction = (direction+4-1)%4;
					break;
				case '0':
					direction = UP;
					break;
			}
			char* room_name = calloc(sizeof(char), 2);
			room_name[0] = output[i];
			// add_room( dag, pos );
			// add_edges( dag, prev_pos, pos );
			mvprintw(pos.y*2, pos.x*2, room_name);
		}

		// walk_dag_and_print

		refresh();
		getch();
		clear();
		endwin();

		free(output);
	}

	return 0;
}
