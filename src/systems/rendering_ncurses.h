#ifndef RENDERING_NCURSES_H 
#define RENDERING_NCURSES_H 

#include "../ecs/components.h"
#include "../ecs/systems.h"

void print_door(struct Point current, struct Point towards,
	struct Point offset, int room_width, int room_height);

void print_current_room_ncurses(struct Graph* dag,
		struct Point current_room, struct Point player);
void center_in_room(struct Point* player);
struct Point move_in_direction(struct Point old_pos, enum Direction direction);
int collision_detection(struct Point current_pos, enum Direction direction);
void position_move(int ch, struct Point* pos);

void rendering_ncurses_init(struct Database_Handle dbh);
void rendering_ncurses_update(struct Database_Handle dbh);
void rendering_ncurses_cleanup(struct Database_Handle dbh);

struct Point get_offset(void);

void print_room_ncurses(struct Graph* dag, int x, int y);
void print_debug_room_info(struct Graph* dag, int x, int y);

static struct subsystem rendering_ncurses = {
	.name = "rendering_ncurses",
	.init_ptr = &rendering_ncurses_init,
	.update_ptr = &rendering_ncurses_update,
	.cleanup_ptr = &rendering_ncurses_cleanup
};

#endif
