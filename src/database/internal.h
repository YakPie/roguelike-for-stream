#ifndef DATABASE_INTERNAL_H
#define DATABASE_INTERNAL_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

enum
{
	DATABASE_MAX_COLUMNS = 255,
	DATABASE_MAX_ROWS = 255
};

enum Datatype_Kind {
   VALUE_KIND,             // Copied based on size
   NULL_TERMINATED_KIND,   // Copied based on null termination
   POINTER_KIND,           // Pointer copied as value, but needs to be freed
   REFERENCE_KIND,         // Pointer copied as value, but not owned / won't be freed
   ENUM_KIND               // Copied as value / int
};

struct Datatype
{
	char *name;
	size_t size;
	enum Datatype_Kind kind; 
};

static struct Datatype datatype_integer = {
	.name = "integer",
	.size = sizeof(int),
	.kind = VALUE_KIND
};

static struct Datatype datatype_uint = {
	.name = "uint",
	.size = sizeof(unsigned int),
	.kind = VALUE_KIND
};

static struct Datatype datatype_size = {
	.name = "size",
	.size = sizeof(size_t),
	.kind = VALUE_KIND
};

static struct Datatype datatype_float = {
	.name = "float",
	.size = sizeof(float),
	.kind = VALUE_KIND
};

static struct Datatype datatype_char = {
	.name = "char",
	.size = sizeof(char),
	.kind = VALUE_KIND
};

static struct Datatype datatype_string = {
	.name = "string",
	.size = sizeof(char),
	.kind = NULL_TERMINATED_KIND 
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

	struct Column columns[DATABASE_MAX_COLUMNS];
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

struct BoundData
{
	void* refrence_to_data;
	struct Column* column;
};

enum TableType
{
	ITERATOR_TABLE_POINTER, // Must be freed
	ITERATOR_TABLE_REFRENCE // Not owned, must NOT be freed
};

enum QueryStatus
{
	QUERYSTATUS_FIRST,
	QUERYSTATUS_VALID_QUERY,
	QUERYSTATUS_INVALID_QUERY
};

struct Iterator
{
	size_t row;
	struct Table* table;
	enum QueryStatus query_status;
	enum TableType table_type;
	struct BoundData bound_data[DATABASE_MAX_COLUMNS];
	size_t number_of_bound_data;
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

struct Table* lookup_table_impl(struct Tables* tables, char const * const name);
struct Table* lookup_table(struct Database_Handle dbh, char const * const name);
struct Table* lookup_virtual_table(struct Database_Handle dbh, char const * const name);
struct Column* lookup_column_impl(struct Table* table, char const * const column_name);
struct Column* lookup_column(struct Database_Handle dbh,
		char const * const table_name, char const * const column_name);
void* get_ptr_column_impl(struct Column* column, size_t row);
size_t column_offset_pr_row(struct Column* column);
void destory_table(struct Database_Handle dbh, char const * const name);
struct Table* vcreate_single_table_impl(char const * const name, size_t num, va_list args);
struct Table* create_single_table_impl(char const * const name, size_t num, ...);
void create_table_impl(
		struct Tables* tables, char const * const name, size_t num, va_list args);
void create_virtual_table(
		struct Database_Handle dbh, char const * const name, size_t num, ...);
void vinsert_into_table_impl(struct Table* table, size_t num, va_list args);
void insert_into_table_impl(struct Table* table, size_t num, ...);
void insert_into_impl(struct Tables* tables, char const * const table_name, size_t num, va_list args);
void update_column(struct Column* column, void * data, size_t row);

#endif
