#ifndef SYSTEMS_DUMMY_SYSTEM_H
#define SYSTEMS_DUMMY_SYSTEM_H

#include "../ecs/systems.h"
#include <ncurses.h>

void subsystem_dummy_system_update(struct Database_Handle dbh);

static struct subsystem dummy_system = {
	.name = "dummy_system",
	.init_ptr = &subsystem_empty_func,
	.update_ptr = &subsystem_dummy_system_update,
	.cleanup_ptr = &subsystem_empty_func
};

#endif
