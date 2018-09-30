#include "frame_counter.h"
#include <ncurses.h>

void subsystem_frame_counter_update(struct Database_Handle dbh)
{
	static int frames = 0;
	frames++;
	mvprintw(1, 10, "Frame %d", frames);
}
