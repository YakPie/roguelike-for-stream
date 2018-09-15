#ifndef DATABASE_H
#define DATABASE_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Lookup table of predefined datatypes

struct Datatype
{
	char *name;
	size_t size;
};

static struct Datatype datatype_integer = {
	.name = "integer",
	.size = sizeof(int),
	.count = 1
};

struct Datatype datatype_char = {
	.name = "char",
	.size = sizeof(char),
	.count = 1
};

struct Column
{
	char *name;
	struct Datatype type;
	size_t count;

	// Only for column oriented layout
	void *data_begin; 
	void *data_current;
	void *data_end;
};

enum DataLayout
{
	DATALAYOUT_ROW_ORIENTED,
	DATALAYOUT_COLOUMN_ORIENTED
};

struct Table
{
	char *name;

	// TODO: rename to schema?
	struct Column columns[255];
	unsigned int number_of_columns;

	enum DataLayout datalayout;
};

struct Tables
{
	struct Table tables[255]; // List of tables
	unsigned int number_of_tables;
};

struct Database_Handle
{
	struct Tables* tables;	
};

struct Query
{
	char *table_name;
	int query_schema;
	// list column names
	// filter method (WHERE)
};

struct Table* lookup_table(
	struct Database_Handle dbh, char* name) {

	for(int i=0; i<dbh.tables->number_of_tables; i++) {
		if(strcmp(dbh.tables->tables[i].name, name) == 0) {
			return &(dbh.tables->tables[i]);
		}
	}

	return NULL;
}

// QUERY / SUBSCRIBE
void query(struct Database_Handle dbh, struct Query query)
{
	if(strcmp(query.table_name, "tables") == 0) {
		printf("tables names\n");
		printf("------------\n");
		for(int i=0; i < dbh.tables->number_of_tables; i++) {
			printf("%s\n", dbh.tables->tables[i].name);
		}
		return;
	}

	if(query.query_schema) {
	// Find a table with the same name as in the query
	for(int i=0; i < dbh.tables->number_of_tables; i++) {
		if(strcmp(dbh.tables->tables[i].name, query.table_name) == 0) {
			// Print out all column names
			printf("column name   datatype name\n");
			printf("-----------   -------------\n");
			for(
				int ic=0;
				ic < dbh.tables->tables[i].number_of_columns;
				ic++) {
				
				printf(
					"%s       %s\n",
					dbh.tables->tables[i].columns[ic].name,
					dbh.tables->tables[i].columns[ic].type.name
				);

			}
			break;
		}
	}
	return;
	}

	struct Table* table = lookup_table(dbh, query.table_name);
	if(table == NULL) {
		fprintf(
			stderr,
			"Couldn't find table with name %s\n",
			query.table_name
		);
		return; 
	}

	for(int i=0; i < table->number_of_columns; i++) {
		printf("Column name '%s'\n", table->columns[i].name);
		void* it = table->columns[i].data_begin;
		void* end = table->columns[i].data_current;
		while(it < end) {
			printf("%d\n", *(int *)it);
			it+= table->columns[i].type.size;
		}
	}
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

// INSERT
struct InsertData
{
	char *name;
	void *data;
};

void insert_into(
		struct Database_Handle dbh, char* table_name,
		int num, ...)
{
	va_list arg_list;
	va_start(arg_list, num);
	for(int i=0; i<num; i++) {
		struct InsertData data = va_arg(arg_list, struct InsertData);
		struct Column* column =
			lookup_column(dbh, table_name, data.name);	

		assert(column != NULL);

		// TOOD: rellaoc if we don't have enough space
		assert(
			column->data_current + column->type.size
			<= column->data_end
		);

		memcpy(column->data_current, data.data, column->type.size);
		column->data_current += column->type.size;
	}
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
		.name = name
	};

	dbh.tables->tables[dbh.tables->number_of_tables] = new_table;
	dbh.tables->tables[dbh.tables->number_of_tables].datalayout =
		DATALAYOUT_COLOUMN_ORIENTED;

	va_list arg_list;
	va_start(arg_list, num);
	for(int i=0; i<num; i++) {
		struct Column* current_column = &(dbh.tables
			->tables[dbh.tables->number_of_tables]
			.columns[i]);

		*current_column = va_arg(arg_list, struct Column);

		void *data = calloc(current_column->type.size, 255);

		current_column->data_begin = data;
		current_column->data_current = data;
		current_column->data_end =
			data + current_column->type.size * 255;
	}
	dbh.tables
			->tables[dbh.tables->number_of_tables]
			.number_of_columns = num;
	va_end(arg_list);

	dbh.tables->number_of_tables++;
}

#endif
