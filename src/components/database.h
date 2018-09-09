#ifndef DATABASE_H
#define DATABASE_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Column
{
	char *name;
	//  size, offset, datatype
	// void *data;
};

struct Table
{
	char *name;

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
	// list column names
	// filter method (WHERE)
};

// QUERY / SUBSCRIBE
void query(struct Database_Handle dbh, struct Query query)
{
	if(strcmp(query.table_name, "tables") == 0) {
		printf("Tables:\n");
		for(int i=0; i < dbh.tables->number_of_tables; i++) {
			printf("- %s\n", dbh.tables->tables[i].name);
		}
	}
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

void create_table(struct Database_Handle dbh, char* name)
{
	assert(dbh.tables != NULL);
	assert(dbh.tables->number_of_tables < 255);

	struct Table new_table = {
		.name = name
	};

	dbh.tables->tables[dbh.tables->number_of_tables] = new_table;
	dbh.tables->number_of_tables++;
}

#endif
