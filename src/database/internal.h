#ifndef DATABASE_INTERNAL_H
#define DATABASE_INTERNAL_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

struct Datatype
{
	char *name;
	size_t size;
};

static struct Datatype datatype_integer = {
	.name = "integer",
	.size = sizeof(int)
};

static struct Datatype datatype_uint = {
	.name = "uint",
	.size = sizeof(unsigned int)
};

static struct Datatype datatype_float = {
	.name = "float",
	.size = sizeof(float)
};

static struct Datatype datatype_char = {
	.name = "char",
	.size = sizeof(char)
};

static struct Datatype datatype_string = {
	.name = "string",
	.size = sizeof(char)
};

struct Column
{
	char *name;
	struct Datatype type;
	size_t count;

	// Only for column oriented layout
	void *data_begin; 
};

enum DataLayout
{
	DATALAYOUT_ROW_ORIENTED,
	DATALAYOUT_COLOUMN_ORIENTED
};

struct Table
{
	char *name;

	struct Column columns[255];
	size_t number_of_columns;

	size_t number_of_rows;
	size_t rows_allocated; // Total space allocated

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
	struct Tables* virtual_tables;
	// TODO: indexes
	// TODO: subscribers
};

struct Query
{
	char *table_name;
	int query_schema;
	// list column names
	// filter method (WHERE)
};

struct Iterator
{
	size_t row;
	struct Table* table;
};

enum IterateStatus
{
	ITERATE_OK,
	ITERATE_END
};

struct InsertData
{
	char *name;
	void *data;
};

struct Table* lookup_table_impl(struct Tables* tables, char* name);
struct Table* lookup_table(struct Database_Handle dbh, char* name);
struct Table* lookup_virtual_table(struct Database_Handle dbh, char* name);
struct Column* lookup_column_impl(struct Table* table, char* column_name);
struct Column* lookup_column(struct Database_Handle dbh, char* table_name, char* column_name);
void* get_ptr_column(struct Table* table, size_t row, size_t i);
void destory_table(struct Database_Handle dbh, char* name);
void create_table_impl(struct Tables* tables, char* name, int num, va_list args);
void create_virtual_table(struct Database_Handle dbh, char* name, int num, ...);

#endif
