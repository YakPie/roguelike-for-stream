#include "../components/components.h"
#include "rendering_ncurses.h"
#include <ncurses.h>
#undef KEY_ENTER

void print_door(struct Point current, struct Point towards,
	struct Point offset, int room_width, int room_height)
{
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

void print_current_room_ncurses(struct Graph* dag, int x, int y, struct Point player) {
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

	for(size_t i=0; i<dag->number_of_edges; i++) {
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

struct Point move_in_direction(struct Point old_pos, enum Direction direction)
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

int collision_detection(struct Point current_pos, enum Direction direction) {
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

void rendering_ncurses_init(struct Database_Handle dbh)
{
	initscr();
	noecho();
	halfdelay(1);
}

void rendering_ncurses_update(struct Database_Handle dbh)
{
	
}

void rendering_ncurses_cleanup(struct Database_Handle dbh)
{
	endwin();
}

struct Point get_offset() {
	int row, col;
	getmaxyx(stdscr, row, col);

	struct Point ret = {
		.x = col / 2,
		.y = row - 10
	};

	return ret;
}

void print_room_ncurses(struct Graph* dag, int x, int y) {
	struct Point offset = get_offset();

	for(size_t i=0; i < dag->number_of_nodes; i++) {
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

	for(size_t i=0; i < dag->number_of_edges; i++) {
		int x = dag->edges[i].from.x + dag->edges[i].to.x,
			 y = dag->edges[i].from.y + dag->edges[i].to.y;

		mvprintw(y + offset.y, x + offset.x, ".");
	}

	mvprintw(y * 2 + offset.y, x * 2 + offset.x, "@");
	refresh();
}

void print_debug_room_info(struct Graph* dag, int x, int y) {
	for(size_t i=0; i<dag->number_of_nodes; i++) {
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

}
