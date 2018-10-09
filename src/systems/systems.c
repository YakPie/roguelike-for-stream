#include "systems.h"
#include "../database/repl.h"
#include "dummy_system.h"
#include "rendering_ncurses.h"
#include "frame_counter.h"
#include "repl_subsystem.h"

void register_subsystem(
		struct Database_Handle dbh,
		char *name,
		subsystem_init init_ptr,
		subsystem_update update_ptr,
		subsystem_cleanup cleanup_ptr
		)
{
	struct InsertData name_data = {
		.name = "name",
		.data = name
	};
	struct InsertData init_ptr_data = {
		.name = "init_ptr",
		.data = &init_ptr
	};
	struct InsertData update_ptr_data = {
		.name = "update_ptr",
		.data = &update_ptr
	};
	struct InsertData cleanup_ptr_data = {
		.name = "cleanup_ptr",
		.data = &cleanup_ptr
	};
	insert_into(dbh, "subsystems", 4, name_data, init_ptr_data, update_ptr_data, cleanup_ptr_data);
}

void register_subsystem_s(struct Database_Handle dbh, struct subsystem s )
{
	register_subsystem(dbh, s.name, s.init_ptr, s.update_ptr, s.cleanup_ptr);
}

void systems_init(struct Database_Handle dbh)
{
	// Create a database table for sub systems
	{
		struct Column name = {
			.name = "name",
			.type = datatype_string,
			.count = 255
		};
		struct Column init_ptr = {
			.name = "init_ptr",
			.type = datatype_init_ptr,
			.count = 1
		};
		struct Column update_ptr = {
			.name = "update_ptr",
			.type = datatype_update_ptr,
			.count = 1
		};
		struct Column cleanup_ptr = {
			.name = "cleanup_ptr",
			.type = datatype_cleanup_ptr,
			.count = 1
		};
		create_table(dbh, "subsystems", 4, name, init_ptr, update_ptr, cleanup_ptr);
	}

	// For each subsystem add them to the database table
	register_subsystem_s(dbh, dummy_system);
	register_subsystem_s(dbh, rendering_ncurses);
	register_subsystem_s(dbh, frame_counter);
	register_subsystem_s(dbh, repl_subsystem);
	
	// Call init on sub_systems?
	{
		struct Iterator it = prepare_query(dbh, "subsystems", 0);
		while(iterate(&it) != ITERATE_END)
		{
			subsystem_init ptr;
			bind_column_data(&it, "init_ptr", &ptr);
			update_bound_data(&it);
			if(ptr != NULL)
				ptr(dbh);
		}
	}
}

void systems_update(struct Database_Handle dbh)
{
	// Select from sub_systems and run update
	struct Iterator it = prepare_query(dbh, "subsystems", 0);
	while(iterate(&it) != ITERATE_END)
	{
		subsystem_update ptr;
		bind_column_data(&it, "update_ptr", &ptr);
		update_bound_data(&it);
		if(ptr != NULL)
			ptr(dbh);
	}
}

void systems_unload(struct Database_Handle dbh)
{
	// Do any logic needed before reloading new version
	// Destory table sub_systems
}

void systems_cleanup(struct Database_Handle dbh)
{
	// Call cleanup on sub_systems?
	struct Iterator it = prepare_query(dbh, "subsystems", 0);
	while(iterate(&it) != ITERATE_END)
	{
		subsystem_cleanup ptr;
		bind_column_data(&it, "cleanup_ptr", &ptr);
		update_bound_data(&it);
		if(ptr != NULL)
			ptr(dbh);
	}

	// Destory table sub_systems
}
