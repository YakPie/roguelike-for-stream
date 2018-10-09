#include "internal.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

struct Table* lookup_table_impl(struct Tables* tables, char const * const name)
{
	for(size_t i=0; i<tables->number_of_tables; i++) {
		if(strcmp(tables->tables[i].name, name) == 0) {
			return &(tables->tables[i]);
		}
	}

	return NULL;
}

struct Table* lookup_table(struct Database_Handle dbh, char const * const name) {
	return lookup_table_impl(dbh.tables, name);
}

struct Table* lookup_virtual_table(struct Database_Handle dbh, char const * const name) {
	return lookup_table_impl(dbh.virtual_tables, name);
}

size_t column_offset_pr_row(struct Column* column)
{
	return column->type.size * column->count;
}

void* get_ptr_column_impl(struct Column* column, size_t row)
{
	size_t offset = row * column_offset_pr_row(column);
	return (char *)column->data_begin + offset;
}

struct Column* lookup_column_impl(struct Table* table, char const * const column_name)
{
	for(size_t i=0; i< table->number_of_columns; i++) {
		if(strcmp(table->columns[i].name, column_name) == 0) {
			return &(table->columns[i]);
		}
	}

	return NULL;
}

struct Column* lookup_column(struct Database_Handle dbh,
		char const * const table_name, char const * const column_name)
{
	return lookup_column_impl(
		lookup_table(dbh, table_name),
		column_name
	);
}

void create_table_impl(struct Tables* tables, char const * const name, size_t num, va_list args)
{
	struct Table new_table = {
		.number_of_rows = 0,
		.rows_allocated = DATABASE_MAX_ROWS,
		.datalayout = DATALAYOUT_ROW_ORIENTED
	};

	// Copying table name inn
	size_t name_len = strlen(name) + 1;
	new_table.name = calloc(name_len, sizeof(char));
	strncpy(new_table.name, name, name_len);

	tables->tables[tables->number_of_tables] = new_table;

	for(size_t i=0; i<num; i++) {
		struct Column* current_column = &(tables
			->tables[tables->number_of_tables]
			.columns[i]);

		*current_column = va_arg(args, struct Column);

		void *data = calloc(DATABASE_MAX_ROWS, current_column->type.size);
		assert(data);
		current_column->data_begin = data;
	}

	tables->tables[tables->number_of_tables]
			.number_of_columns = num;

	tables->number_of_tables++;
}

void create_virtual_table(struct Database_Handle dbh, char const * const name, size_t num, ...)
{
	assert(dbh.tables != NULL);
	assert(dbh.tables->number_of_tables < 255);

	va_list args;
	va_start(args, num);
	create_table_impl(dbh.virtual_tables, name, num, args);
	va_end(args);
}

void destory_table(struct Database_Handle dbh, char const * const name)
{
	for(size_t i_t=0; i_t<dbh.tables->number_of_tables; i_t++) {
		if(strcmp(dbh.tables->tables[i_t].name, name) == 0) {
			// TODO (#34): Create a method for destorying a table 
		}
	}
}

void insert_into_impl(struct Tables* tables, char const * const table_name, size_t num, va_list args)
{
	struct Table* table = lookup_table_impl(tables, table_name);
	for(size_t i=0; i<num; i++) {
		struct InsertData data = va_arg(args, struct InsertData);
		struct Column* column = lookup_column_impl(table, data.name);	

		assert(column != NULL);
		// TODO: rellaoc if we don't have enough space
		assert(table->number_of_rows < table->rows_allocated);

		update_column(column, data.data, table->number_of_rows);
	}

	table->number_of_rows++;
}

void update_column(struct Column* column, void * data, size_t row)
{	
	size_t column_size = column->type.size * column->count;
	void* dst = (char*)column->data_begin + column_size * row;

	switch(column->type.kind) {
		case NULL_TERMINATED_KIND:
			strncpy(dst, data, column_size);
			break;
		default:
			memcpy(dst, data, column_size);
			break;
	};
}
