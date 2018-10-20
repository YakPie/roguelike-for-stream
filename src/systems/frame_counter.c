#include "../ecs/systems.h"
#include <ncurses.h>

static void subsystem_frame_counter_update(struct Database_Handle dbh)
{
	(void) dbh;

	static int frames = 0;
	frames++;
	mvprintw(1, 10, "Frame %d", frames);
}

extern struct subsystem frame_counter_subsystem;
extern struct subsystem frame_counter_subsystem = {
	.name = "frame_counter",
	.init_ptr = &subsystem_empty_func,
	.update_ptr = &subsystem_frame_counter_update,
	.cleanup_ptr = &subsystem_empty_func
};
