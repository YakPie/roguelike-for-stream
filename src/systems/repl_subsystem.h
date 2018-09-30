#ifndef repl_subsystem_h
#define repl_subsystem_h

#include "systems.h"

void repl_subsystem_update(struct Database_Handle dbh);

static struct subsystem repl_subsystem = {
	.name = "repl",
	.init_ptr = &subsystem_empty_func,
	.update_ptr = &repl_subsystem_update,
	.cleanup_ptr = &subsystem_empty_func
};

#endif
