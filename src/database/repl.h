#ifndef DATABASE_REPLE_H
#define DATABASE_REPLE_H

#include "core.h"

void print_column(void *it, struct Datatype dt);
void print_column_headers(struct Iterator it);
void print_column_data(struct Iterator it);

int repl(struct Database_Handle dbh);

#endif
