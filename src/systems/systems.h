#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "../database/core.h"

typedef void (*subsystem_init)(struct Database_Handle dbh);
typedef void (*subsystem_update)(struct Database_Handle dbh);
typedef void (*subsystem_unload)(struct Database_Handle dbh);
typedef void (*subsystem_cleanup)(struct Database_Handle dbh);

static void subsystem_empty_func(struct Database_Handle dbh) {}

static struct Datatype datatype_init_ptr = {
	.name = "init_ptr",
	.size = sizeof(subsystem_init)
};

static struct Datatype datatype_update_ptr = {
	.name = "update_ptr",
	.size = sizeof(subsystem_update)
};

static struct Datatype datatype_cleanup_ptr = {
	.name = "cleanup_ptr",
	.size = sizeof(subsystem_cleanup)
};

void systems_init(struct Database_Handle dbh);
void systems_update(struct Database_Handle dbh);
void systems_unload(struct Database_Handle dbh);
void systems_cleanup(struct Database_Handle dbh);

#endif
