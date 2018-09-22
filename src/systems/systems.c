#include "systems.h"
#include "../database/repl.h"
#include "dummy_system.h"
#include "rendering_ncurses.h"

void systems_init(struct Database_Handle dbh)
{
	// Create a database table for sub systems
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
	create_table(dbh, "subsystems", 3, name, init_ptr, update_ptr);

	// For each subsystem add them to the database table
	// Insert dummy system
	{
		struct InsertData name_data = {
			.name = "name",
			.data = "dummy system"
		};
		subsystem_init init_ptr = &subsystem_empty_func;
		struct InsertData init_ptr_data = {
			.name = "init_ptr",
			.data = &init_ptr
		};
		subsystem_update dummy_system_update_ptr = &subsystem_dummy_system_update;	
		struct InsertData update_ptr_data = {
			.name = "update_ptr",
			.data = &dummy_system_update_ptr
		};
		insert_into(dbh, "subsystems", 3, name_data, init_ptr_data, update_ptr_data);
	}
	// Insert rendering ncurses system
	{
		struct InsertData name_data = {
			.name = "name",
			.data = "rendering ncurses"
		};
		subsystem_init init_ptr = &rendering_ncurses_init;
		struct InsertData init_ptr_data = {
			.name = "init_ptr",
			.data = &init_ptr
		};
		subsystem_update update_ptr = &rendering_ncurses_update;	
		struct InsertData update_ptr_data = {
			.name = "update_ptr",
			.data = &update_ptr
		};
		insert_into(dbh, "subsystems", 3, name_data, init_ptr_data, update_ptr_data);
	}

	// Call init on sub_systems?
	{
		struct Query query_subsystems = {
			.table_name = "subsystems"
		};
		struct Iterator it = query(dbh, query_subsystems);
		do {
			for(int i=0; i < it.table->number_of_columns; i++) {
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
		for(int i=0; i < it.table->number_of_columns; i++) {
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
}

void systems_cleanup(struct Database_Handle dbh)
{
	// Call cleanup on sub_systems?
	// Destory table sub_systems
}
