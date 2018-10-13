#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "core.h"
#include "query.h"

struct Iterator query(struct Database_Handle dbh, struct Query query)
{
	struct Iterator it = {
		.row = 0,
		.query_status = QUERYSTATUS_INVALID_QUERY,
		.table_type = ITERATOR_TABLE_REFRENCE
	};

	it.table = lookup_virtual_table(dbh, query.table_name);		
	if(it.table == NULL) {

		it.table = lookup_table(dbh, query.table_name);
		if(it.table == NULL) {
			fprintf(
				stderr,
				"Couldn't find table with name %s\n",
				query.table_name
			);
			return it; 
		}
	}

	it.query_status = QUERYSTATUS_FIRST;

	if(query.query_schema) {
		// TODO: implement schema querying
		fprintf(stderr, "Querying schema is not yet implemented\n");
		it.query_status = QUERYSTATUS_INVALID_QUERY;
	}

	return it;
}

enum IterateStatus iterate(struct Iterator* it)
{
	if(it->query_status == QUERYSTATUS_INVALID_QUERY)
		return ITERATE_END;

	if(it->query_status == QUERYSTATUS_FIRST) {
		it->query_status = QUERYSTATUS_VALID_QUERY;
		if(it->row < it->table->number_of_rows)
			return ITERATE_OK;
		else
			return ITERATE_END;
	}

	if(it->row + 1 < it->table->number_of_rows) {
		it->row++;
		return ITERATE_OK;
	}

	return ITERATE_END;
}

void insert_into(struct Database_Handle dbh, char const * const table_name, size_t num, ...)
{
	va_list arg_list;
	va_start(arg_list, num);
	insert_into_impl(dbh.tables, table_name, num, arg_list);
	va_end(arg_list);
}

void insert_into_virtual_table(
		struct Database_Handle dbh, char const * const table_name, size_t num, ...)
{
	va_list arg_list;
	va_start(arg_list, num);
	insert_into_impl(dbh.virtual_tables, table_name, num, arg_list);
	va_end(arg_list);
}


struct Database_Handle new_database()
{
	struct Database_Handle dbh = {0};
	dbh.tables         = calloc(1, sizeof(struct Tables)); 
	assert(dbh.tables);
	dbh.virtual_tables = calloc(1, sizeof(struct Tables)); 
	assert(dbh.virtual_tables);

	// setup for the virtual table for holding table information
	{
		struct Column name = {
			.name = "name",
			.type = datatype_string,
			.count = 255
		};
		create_virtual_table(dbh, "tables", 1, name);	
	}

	return dbh;
}

void create_table(struct Database_Handle dbh, char const * const name, size_t num, ...)
{
	assert(dbh.tables != NULL);
	assert(dbh.tables->number_of_tables < 255);

	va_list args;
	va_start(args, num);
	create_table_impl(dbh.tables, name, num, args);
	va_end(args);

	// Update virtual table
	{
		struct InsertData name_data = {
			.name = "name",
		};

		size_t name_len = strlen(name)+1;
		name_data.data = calloc(name_len, sizeof(char));
		strncpy(name_data.data, name, name_len);

		insert_into_virtual_table(dbh, "tables", 1, name_data);
	}
}

struct Iterator prepare_query(
		struct Database_Handle dbh, char const * const query_string, size_t num, ...)
{
	/*
	struct Query q = {0};

	size_t query_string_len = strlen(query_string) + 1;
	q.table_name = calloc(query_string_len, sizeof(char));
	strncpy(q.table_name, query_string, query_string_len);
	//*/
	struct Query q = parse_query(query_string);

	return query(dbh, q);
}

void bind_column_data(
		struct Iterator* it, char const * const column_name, void* bound_variable)
{
	struct Column* column = lookup_column_impl(it->table, column_name);	
	struct BoundData bd = {
		.refrence_to_data = bound_variable,
		.column = column
	};
	it->bound_data[it->number_of_bound_data] = bd;
	it->number_of_bound_data++;
}

void update_bound_data(struct Iterator* it)
{
	for(size_t i=0; i < it->number_of_bound_data; i++) {
		void* data = get_ptr_column_impl(it->bound_data[i].column, it->row);
		memcpy(
			it->bound_data[i].refrence_to_data, 
			data,
			column_offset_pr_row(it->bound_data[i].column)
		);
	}
}

void free_iterator(struct Iterator it)
{
	if(it.table_type == ITERATOR_TABLE_POINTER) {
		free(it.table);
	}
}
