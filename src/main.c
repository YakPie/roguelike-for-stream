#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "components/components.h"
#include "systems/pcg_dungeon.h"

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

struct Graph* create_dag_from_dungeonrule(char *output)
{
	struct Point pos = {
		.x = 0,
		.y = 0
	};

	struct Point tmp_pos = pos;
	struct Point prev_pos = pos;
	struct LinkedList* stack = malloc(sizeof(struct LinkedList));
	stack->point = tmp_pos;
	stack->next = NULL; 

	int direction = 0;
	struct Graph* dag = calloc(sizeof(struct Graph), 1);

	for(int i=0; i < strlen(output); i++) {
		prev_pos = pos;

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

		add_room( dag, pos, output[i] );
		add_edges( dag, prev_pos, pos );
	}

	return dag;
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
