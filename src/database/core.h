#ifndef DATABASE_H
#define DATABASE_H

#include "internal.h"

struct Database_Handle new_database();
void create_table(struct Database_Handle dbh, char* name, int num, ...);
void insert_into(struct Database_Handle dbh, char* table_name, int num, ...);
struct Iterator query(struct Database_Handle dbh, struct Query query);
enum IterateStatus iterate(struct Iterator* it);

// UPDATE
// DELETE

#endif
