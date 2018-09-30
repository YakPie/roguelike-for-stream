#include "repl_subsystem.h"
#include "../components/components.h"
#include "../database/repl.h"
#include <ncurses.h>

void repl_subsystem_update(struct Database_Handle dbh)
{
	int gamestate;
	struct Query q = {
		.table_name = "gamestate"
	};
	struct Iterator it = query(dbh, q);
	struct Column* gamestate_column = lookup_column_impl(it.table, "gamestate");	
	gamestate = *(int*) gamestate_column->data_begin;

	if(gamestate == GAMESTATE_REPL) {
		refresh();
		def_prog_mode();   // Save ncurses setting
		endwin();          // Quit ncurses mode
		setvbuf(stdout, NULL, _IOLBF, 0); // Reset stdout

		// Enter REPL
		while(repl(dbh));

		reset_prog_mode(); // Get back to ncurses
		refresh();

		// Update database table
		gamestate = GAMESTATE_MAP;
		update_column(gamestate_column, &gamestate, 0);
	}
}
