#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>
#undef KEY_ENTER

#include "components/components.h"
#include "database/core.h"
#include "database/repl.h"
#include "systems/systems.h"
#include "systems/pcg_dungeon.h"
#include "systems/rendering_ncurses.h"
#include "systems/grammer_parser.h"

int main(int argc, char **argv)
{
	srand(time(NULL));

	if(argc < 2) {
		printf("takes a level\n");
		exit(1);
	}

	FILE *fd = fopen(argv[1], "r");
	struct RulesWrapper rw = parser(fd);
	fclose(fd);

	char* starting_rules = "se";
	if(argc > 2) {
		starting_rules = argv[2];
	}

	int num_replacements = 10;
	if(argc > 3) {
		num_replacements = atoi(argv[3]);
	}

	// Initialize database
	struct Database_Handle dbh = new_database();

	// Initialize systems
	systems_init(dbh);

	int ch;
	struct Graph* dag = NULL;

	struct Point current_room = {
		.x = 0,
		.y = 0
	};
	struct Point position_in_room = {
		.x = 0,
		.y = 0
	};
	/*
	struct Column entityid_column = {
		.name = "entityid",
		.type = datatype_uint,
		.count = 1
	};

	// Create table for position
	{
		struct Column x_column = {
			.name = "x",
			.type = datatype_integer,
			.count = 1
		};
		struct Column y_column = {
			.name = "y",
			.type = datatype_integer,
			.count = 1
		};
		create_table(dbh, "position", 3, entityid_column, x_column, y_column);
	}

	// Create table for rendering
	{
		struct Column rendering_char = {
			.name = "rendering_char",
			.type = datatype_char,
			.count = 1
		};
		create_table(dbh, "rendering", 2, entityid_column, rendering_char);
	}
	//*/
	// Create table for gamestate
	{
		struct Column gamestate_column = {
			.name = "gamestate",
			.type = datatype_integer,
			.count = 1
		};
		create_table(dbh, "gamestate", 1, gamestate_column);
		
		int gamestate = GAMESTATE_MAP;
		struct InsertData gamestate_data = {
			.name = "gamestate",
			.data = &gamestate
		};
		insert_into(dbh, "gamestate", 1, gamestate_data);
	}

	struct Iterator it = prepare_query(dbh, "gamestate", 0);

	int gamestate;
	bind_column_data(&it, "gamestate", &gamestate);
	update_bound_data(&it);

	while(gamestate != GAMESTATE_END) {
		systems_update(dbh);
		update_bound_data(&it);

		if(dag == NULL || ch == KEY_ENTER) {
			if(dag != NULL) free(dag);

			char* output = rule_engine(
				rw.rules,
				starting_rules,
				num_replacements,
				rw.number_of_rules
			);

			// Travel the dungeon rule and gennerates a DAG
			dag = create_dag_from_dungeonrule(output);
			free(output);

			// Finds starting room
			for(size_t i=0; i<dag->number_of_nodes; i++) {
				if(dag->nodes[i].type == 's') {
					current_room.x = dag->nodes[i].position.x;
					current_room.y = dag->nodes[i].position.y;
					center_in_room(&position_in_room);
					break;
				}
			}
		}

		switch(gamestate)
		{
			case GAMESTATE_MAP:
				print_room_ncurses(dag, current_room.x, current_room.y);
				break;
			case GAMESTATE_ROOM:
				print_current_room_ncurses(
					dag,
					current_room.x, current_room.y,
					position_in_room
				);
				break;
		}

		ch = getch();
		clear();

		if(gamestate == GAMESTATE_MAP) {
			// move in map
			position_move(ch, &current_room);
		} else if(gamestate == GAMESTATE_ROOM) {
			// move in room
			position_move(ch, &position_in_room);
		}

		switch(ch) {
			case KEY_M:
				gamestate = gamestate == GAMESTATE_MAP ? GAMESTATE_ROOM : GAMESTATE_MAP;
				break;
			case KEY_ESC:
				gamestate = GAMESTATE_END;
				break;
			case KEY_R:
				gamestate = GAMESTATE_REPL;
				break;
		}

		update_column(lookup_column(dbh, "gamestate", "gamestate"), &gamestate, 0);
	}

	free(dag);

	systems_cleanup(dbh);

	return 0;
}
