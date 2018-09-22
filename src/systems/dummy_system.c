#include "dummy_system.h"
#include <stdio.h>

void subsystem_dummy_system_update(struct Database_Handle dbh)
{
	mvprintw(4, 20, "subsystem dummy system");
	//printf("this is printed from dummy system\n");
}
