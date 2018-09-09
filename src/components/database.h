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
	// TODO: think about alignment
	// how to print
};

struct Column
{
	char *name;
	struct Datatype type;
	// void *data;
};

struct Table
{
	char *name;

	// TODO: rename to schema?
	struct Column columns[255];
	unsigned int number_of_columns;

	// column oriented tables vs row oriented tables

	// void *data;
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

// QUERY / SUBSCRIBE
void query(struct Database_Handle dbh, struct Query query)
{
	if(strcmp(query.table_name, "tables") == 0) {
		printf("tables names\n");
		printf("------------\n");
		for(int i=0; i < dbh.tables->number_of_tables; i++) {
			printf("%s\n", dbh.tables->tables[i].name);
		}
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

	}

	// Query data
}

// INSERT
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

	va_list arg_list;
	va_start(arg_list, num);
	for(int i=0; i<num; i++) {
		dbh.tables
			->tables[dbh.tables->number_of_tables]
			.columns[i] = va_arg(arg_list, struct Column);
	}
	dbh.tables
			->tables[dbh.tables->number_of_tables]
			.number_of_columns = num;
	va_end(arg_list);

	dbh.tables->number_of_tables++;
}

#endif
