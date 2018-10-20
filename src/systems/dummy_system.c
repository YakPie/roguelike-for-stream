#include "../ecs/systems.h"
#include <stdio.h>
#include <ncurses.h>

static void subsystem_dummy_system_update(struct Database_Handle dbh)
{
	(void) dbh;

	mvprintw(3, 10, "subsystem dummy system");
}

extern struct subsystem dummy_subsystem;
extern struct subsystem dummy_subsystem = {
	.name = "dummy_system",
	.init_ptr = &subsystem_empty_func,
	.update_ptr = &subsystem_dummy_system_update,
	.cleanup_ptr = &subsystem_empty_func
};
