#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "core.h"

struct Iterator query(struct Database_Handle dbh, struct Query query)
{
	struct Iterator it = {
		.row = 0
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

	if(query.query_schema) {
		// Print out all column names
		printf("column name   datatype name\n");
		printf("-----------   -------------\n");
		for(int i=0; i < it.table->number_of_columns; i++) {
			printf("%s       %s\n",
					it.table->columns[i].name,
					it.table->columns[i].type.name
					);

		}
		return it;
	}

	return it;
}

enum IterateStatus iterate(struct Iterator* it)
{
	if(it->row + 1 < it->table->number_of_rows) {
		it->row++;
		return ITERATE_OK;
	}

	return ITERATE_END;
}

void insert_into_impl(struct Tables* tables, char* table_name,int num, va_list args)
{
	struct Table* table = lookup_table_impl(tables, table_name);
	for(int i=0; i<num; i++) {
		struct InsertData data = va_arg(args, struct InsertData);
		struct Column* column = lookup_column_impl(table, data.name);	

		assert(column != NULL);
		size_t column_size = column->type.size * column->count;

		// TODO: rellaoc if we don't have enough space
		assert(
			table->number_of_rows < table->rows_allocated
		);

		void* data_current = column->data_begin + column_size * table->number_of_rows;
		memcpy(data_current, data.data, column_size);
	}

	table->number_of_rows++;
}

void insert_into(struct Database_Handle dbh, char* table_name, int num, ...)
{
	va_list arg_list;
	va_start(arg_list, num);
	insert_into_impl(dbh.tables, table_name, num, arg_list);
	va_end(arg_list);
}

void insert_into_virtual_table(struct Database_Handle dbh, char* table_name, int num, ...)
{
	va_list arg_list;
	va_start(arg_list, num);
	insert_into_impl(dbh.virtual_tables, table_name, num, arg_list);
	va_end(arg_list);
}


struct Database_Handle new_database()
{
	struct Database_Handle dbh = {};
	dbh.tables         = calloc(sizeof(struct Tables), 1); 
	dbh.virtual_tables = calloc(sizeof(struct Tables), 1); 

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

void create_table(struct Database_Handle dbh, char* name, int num, ...)
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
			.data = name
		};
		insert_into_virtual_table(dbh, "tables", 1, name_data);
	}
}

