#include "systems.h"
#include "../database/repl.h"
#include "dummy_system.h"
#include "rendering_ncurses.h"
#include "frame_counter.h"

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
	register_subsystem(
		dbh,
		"dummy system",
		&subsystem_empty_func,
		&subsystem_dummy_system_update,
		&subsystem_empty_func
	);
	register_subsystem(
		dbh,
		"rendering_ncurses",
		&rendering_ncurses_init,
		&rendering_ncurses_update,
		&rendering_ncurses_cleanup
	);
	register_subsystem(
		dbh,
		"frame_counter",
		&subsystem_empty_func,
		&subsystem_frame_counter_update,
		&subsystem_empty_func
	);
	
	// Call init on sub_systems?
	{
		struct Query query_subsystems = {
			.table_name = "subsystems"
		};
		struct Iterator it = query(dbh, query_subsystems);
		do {
			for(size_t i=0; i < it.table->number_of_columns; i++) {
				if(strcmp(it.table->columns[i].name, "init_ptr") == 0) {
					 subsystem_init ptr = *(subsystem_init*) get_ptr_column(it.table, it.row, i);
					 if(ptr != NULL)
						ptr(dbh);
					 break;
				}
			}
		} while(iterate(&it) != ITERATE_END);
	}
}

void systems_update(struct Database_Handle dbh)
{
	// Select from sub_systems and run update
	struct Query query_subsystems = {
		.table_name = "subsystems"
	};
	struct Iterator it = query(dbh, query_subsystems);
	do {
		for(size_t i=0; i < it.table->number_of_columns; i++) {
			if(strcmp(it.table->columns[i].name, "update_ptr") == 0) {
				 subsystem_update ptr = *(subsystem_update*) get_ptr_column(it.table, it.row, i);
				 if(ptr != NULL)
				 	ptr(dbh);
				 break;
			}
		}
	} while(iterate(&it) != ITERATE_END);
}

void systems_unload(struct Database_Handle dbh)
{
	// Do any logic needed before reloading new version
	// Destory table sub_systems
}

void systems_cleanup(struct Database_Handle dbh)
{
	// Call cleanup on sub_systems?
	{
		struct Query query_subsystems = {
			.table_name = "subsystems"
		};
		struct Iterator it = query(dbh, query_subsystems);
		do {
			for(size_t i=0; i < it.table->number_of_columns; i++) {
				if(strcmp(it.table->columns[i].name, "cleanup_ptr") == 0) {
					 subsystem_cleanup ptr = *(subsystem_cleanup*) get_ptr_column(it.table, it.row, i);
					 if(ptr != NULL)
						ptr(dbh);
					 break;
				}
			}
		} while(iterate(&it) != ITERATE_END);
	}
	// Destory table sub_systems
}
