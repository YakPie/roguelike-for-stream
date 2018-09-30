#ifndef SUBSYSTEM_FRAME_COUNTER_H
#define SUBSYSTEM_FRAME_COUNTER_H

#include "systems.h"

void subsystem_frame_counter_update(struct Database_Handle dbh);

static struct subsystem frame_counter = {
	.name = "frame_counter",
	.init_ptr = &subsystem_empty_func,
	.update_ptr = &subsystem_frame_counter_update,
	.cleanup_ptr = &subsystem_empty_func
};

#endif
