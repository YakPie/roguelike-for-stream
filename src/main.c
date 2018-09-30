#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>
#undef KEY_ENTER

#include "components/components.h"
#include "database/core.h"
#include "database/repl.h"
#include "systems/systems.h"
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
	if(towards.x == current.x) {
		// upwards
		if(towards.y < current.y) {
			y = room_height;
			x = room_width / 2;
		}
		// downwards
		else if(towards.y > current.y) {
			y = 0;
			x = room_width / 2;
		}
		else
			return;

		mvprintw(-y + offset.y, x + offset.x - 2, "|   |");
	}
	else if(towards.y == current.y) {
		// left
		if(towards.x < current.x) {
			y = room_height / 2;
			x = 0;
		}
		// right
		else if(towards.x > current.y) {
			y = room_height / 2;
			x = room_width;
		}
		else
			return;

		mvprintw(-y + offset.y - 2, x + offset.x, "-");
		mvprintw(-y + offset.y - 1, x + offset.x, " ");
		mvprintw(-y + offset.y + 0, x + offset.x, " ");
		mvprintw(-y + offset.y + 1, x + offset.x, " ");
		mvprintw(-y + offset.y + 2, x + offset.x, "-");
	}
}

void print_current_room_ncurses(struct Graph* dag, int x, int y,
		struct Point player) {
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

	mvprintw(player.y + offset.y, player.x + offset.x, "@");
}

void center_in_room(struct Point* player)
{
	int row, col;
	getmaxyx(stdscr, row, col);

	// Print Walls
	int room_width = col / 2,
		 room_height = row / 2;

	player->x = room_width / 2;
	player->y = -room_height / 2;
}

#define KEY_D 100
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_M 109
#define KEY_R 114
#define KEY_ENTER 10
#define KEY_ESC 27

struct Point move_in_direction(
	struct Point old_pos, enum Direction direction)
{
	switch(direction) {
		case UP: {
			struct Point pos = {
				.x = old_pos.x,
				.y = old_pos.y - 1
			};
			return pos;
		}
		case DOWN: {
			struct Point pos = {
				.x = old_pos.x,
				.y = old_pos.y + 1
			};
			return pos;
		}
		case LEFT: {
			struct Point pos = {
				.x = old_pos.x - 1,
				.y = old_pos.y
			};
			return pos;
		}
		case RIGHT: {
			struct Point pos = {
				.x = old_pos.x + 1,
				.y = old_pos.y
			};
			return pos;
		}
	}

	return old_pos;
}

int collision_detection(
		struct Point current_pos, enum Direction direction) {
	/*
	struct Point new_pos = move_in_direction(
		current_pos,
		direction
	);

	* Example query:
	 * new_pos(x, y),
	 * position(entity_id, x, y),
	 * collision(entity_id, true)?
	 */

	return 0; // No collision found
}

void position_move(char ch, struct Point* pos) {
	switch(ch) {
		case KEY_W:
			mvprintw(5, 10, "KEY_UP!");
			if(collision_detection(*pos, UP))
				break;

			pos->y--;
			break;
		case KEY_S:
			mvprintw(5, 10, "KEY_DOWN!");
			if(collision_detection(*pos, DOWN))
				break;

			pos->y++;
			break;
		case KEY_A:
			mvprintw(5, 10, "KEY_LEFT!");
			if(collision_detection(*pos, LEFT))
				break;

			pos->x--;
			break;
		case KEY_D:
			mvprintw(5, 10, "KEY_RIGHT!");
			if(collision_detection(*pos, RIGHT))
				break;

			pos->x++;
			break;
		default:
			mvprintw(5, 10, "%d", ch);
	}
}

enum GameState
{
	GAMESTATE_END,
	GAMESTATE_MAP,
	GAMESTATE_ROOM,
	GAMESTATE_REPL
};

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

	// Initialize database
	struct Database_Handle dbh = new_database();

	// Initialize systems
	systems_init(dbh);

	// Setup ncurses
	int ch;
	char* output = NULL;
	struct Graph* dag = NULL;

	struct Point current_room = {
		.x = 0,
		.y = 0
	};
	struct Point position_in_room = {
		.x = 0,
		.y = 0
	};

	int gamestate = GAMESTATE_MAP;
	while(gamestate != GAMESTATE_END) {
		systems_update(dbh);

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

			// Finds starting room
			for(int i=0; i<dag->number_of_nodes; i++) {
				if(dag->nodes[i].type == 's') {
					current_room.x = dag->nodes[i].position.x;
					current_room.y = dag->nodes[i].position.y;
					center_in_room(&position_in_room);
					break;
				}
			}
		}

		switch(gamestate)
		{
			case GAMESTATE_MAP:
				print_room_ncurses(dag, current_room.x, current_room.y);
				break;
			case GAMESTATE_ROOM:
				print_current_room_ncurses(
					dag,
					current_room.x, current_room.y,
					position_in_room
				);
				break;
			case GAMESTATE_REPL:
				refresh();
				def_prog_mode();   // Save ncurses setting
				endwin();          // Quit ncurses mode
				setvbuf(stdout, NULL, _IOLBF, 0); // Reset stdout

				// Enter REPL
				while(repl(dbh));

				reset_prog_mode(); // Get back to ncurses
				refresh();
				gamestate = GAMESTATE_MAP;
				break;
		}

		ch = getch();
		clear();

		if(gamestate == GAMESTATE_MAP) {
			// move in map
			position_move(ch, &current_room);
		} else if(gamestate == GAMESTATE_ROOM) {
			// move in room
			position_move(ch, &position_in_room);
		}

		switch(ch) {
			case KEY_M:
				gamestate = gamestate == GAMESTATE_MAP ? GAMESTATE_ROOM : GAMESTATE_MAP;
				break;
			case KEY_ESC:
				gamestate = GAMESTATE_END;
				break;
			case KEY_R:
				gamestate = GAMESTATE_REPL;
				break;
		}
	}

	free(output);
	free(dag);

	systems_cleanup(dbh);
	printf("End game\n");

	return 0;
}
