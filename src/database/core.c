#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "core.h"

struct Table* lookup_table(
	struct Database_Handle dbh, char* name) {

	for(int i=0; i<dbh.tables->number_of_tables; i++) {
		if(strcmp(dbh.tables->tables[i].name, name) == 0) {
			return &(dbh.tables->tables[i]);
		}
	}

	return NULL;
}

void print_column(void *it, struct Datatype dt) {
	if(strcmp(dt.name, "integer") == 0) {
		printf("%d\t", *(int *)it);
	} else if(strcmp(dt.name, "float") == 0) {
		printf("%f\t", *(float *)it);
	} else if(strcmp(dt.name, "char") == 0) {
		printf("%c\t", *(char *)it);
	} else if(strcmp(dt.name, "string") == 0) {
		printf("%s\t", (char *)it);
	}
}

void* get_ptr_column(struct Table* table, size_t row, size_t i)
{
	return table->columns[i].data_begin
		  + row * table->columns[i].type.size *
		  table->columns[i].count;
}

void print_column_headers(struct Iterator it) {
	for(int i=0; i < it.table->number_of_columns; i++) {
		printf("%s\t", it.table->columns[i].name);
	}
	printf("\n");
}


// QUERY / SUBSCRIBE
struct Iterator query(struct Database_Handle dbh, struct Query query)
{
	struct Iterator it = {
		.row = 0
	};

	if(strcmp(query.table_name, "tables") == 0) {
		printf("tables names\n");
		printf("------------\n");
		for(int i=0; i < dbh.tables->number_of_tables; i++) {
			printf("%s\n", dbh.tables->tables[i].name);
		}
		return it;
	}

	it.table = lookup_table(dbh, query.table_name);
	if(it.table == NULL) {
		fprintf(
			stderr,
			"Couldn't find table with name %s\n",
			query.table_name
		);
		return it; 
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

	// Printing out column headers
	print_column_headers(it);

	// Printing out data
	enum IterateStatus it_status = ITERATE_OK;
	while(it_status != ITERATE_END)
	{
		for(int i=0; i < it.table->number_of_columns; i++) {
			print_column(
				get_ptr_column(it.table, it.row, i),
				it.table->columns[i].type
			);
		}
		printf("\n");
		it_status = iterate(&it);
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


struct Column* lookup_column(
	struct Database_Handle dbh, char* table_name, char* column_name)
{
	for(int i=0; i<dbh.tables->number_of_tables; i++) {
		if(strcmp(dbh.tables->tables[i].name, table_name) == 0) {
			
			for(
				int ic=0;
				ic<dbh.tables->tables[i].number_of_columns;
				ic++
			) {
				if(strcmp(
						dbh.tables->tables[i].columns[ic].name,
						column_name
				) == 0) {
					
					return &(dbh.tables->tables[i].columns[ic]);

				}
			}

		}
	}

	return NULL;
}

void insert_into(
		struct Database_Handle dbh, char* table_name,
		int num, ...)
{
	va_list arg_list;
	va_start(arg_list, num);
	struct Table* table = lookup_table(dbh, table_name);
	for(int i=0; i<num; i++) {
		struct InsertData data = va_arg(arg_list, struct InsertData);
		struct Column* column =
			lookup_column(dbh, table_name, data.name);	

		assert(column != NULL);
		size_t column_size = column->type.size * column->count;

		// TOOD: rellaoc if we don't have enough space
		assert(
			table->number_of_rows < table->rows_allocated
		);

		void* data_current =
			column->data_begin + column_size * table->number_of_rows;
		memcpy(data_current, data.data, column_size);
	}

	table->number_of_rows++;
	va_end(arg_list);
}

// DELETE
// UPDATE

struct Database_Handle new_database()
{
	struct Database_Handle dbh = {};
	dbh.tables = calloc(sizeof(struct Tables), 1); 
	return dbh;
}

void create_table(
		struct Database_Handle dbh, char* name, int num, ...)
{
	assert(dbh.tables != NULL);
	assert(dbh.tables->number_of_tables < 255);

	struct Table new_table = {
		.name = name,
		.number_of_rows = 0,
		.rows_allocated = 255,
		.datalayout = DATALAYOUT_ROW_ORIENTED
	};

	dbh.tables->tables[dbh.tables->number_of_tables] = new_table;

	va_list arg_list;
	va_start(arg_list, num);
	for(int i=0; i<num; i++) {
		struct Column* current_column = &(dbh.tables
			->tables[dbh.tables->number_of_tables]
			.columns[i]);

		*current_column = va_arg(arg_list, struct Column);

		void *data = calloc(current_column->type.size, 255);

		current_column->data_begin = data;
	}
	dbh.tables
			->tables[dbh.tables->number_of_tables]
			.number_of_columns = num;
	va_end(arg_list);

	dbh.tables->number_of_tables++;
}
