#ifndef DATABASE_H
#define DATABASE_H

#include "internal.h"

struct Database_Handle new_database(void);
void create_table(struct Database_Handle dbh, char const * const name, size_t num, ...);
void insert_into(struct Database_Handle dbh, char const * const table_name, size_t num, ...);

// Query
struct Iterator query(struct Database_Handle dbh, struct Query query);
struct Iterator prepare_query(struct Database_Handle dbh, char const * const query, size_t num, ...);
enum IterateStatus iterate(struct Iterator* it);
void free_iterator(struct Iterator it);

// Data binding
void bind_column_data(struct Iterator* it, char const * const column_name, void* bound_variable);
void update_bound_data(struct Iterator* it);

#endif
