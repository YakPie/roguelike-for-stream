#include "repl.h"
#include <stdio.h>

void print_column(void *it, struct Datatype dt) {
	if(strcmp(dt.name, "integer") == 0) {
		printf("%10d\t", *(int *)it);
	} else if(strcmp(dt.name, "uint") == 0) {
		printf("%10u\t", *(unsigned int *)it);
	} else if(strcmp(dt.name, "float") == 0) {
		printf("%10f\t", *(double *)it);
	} else if(strcmp(dt.name, "char") == 0) {
		printf("%10c\t", *(char *)it);
	} else if(strcmp(dt.name, "string") == 0) {
		printf("%10s\t", (char *)it);
	} else {
		printf("<%8s>\t", dt.name);
	}
}

void print_column_headers(struct Iterator it) {
	if(it.query_status == QUERYSTATUS_INVALID_QUERY)
		return;

	for(size_t i=0; i < it.table->number_of_columns; i++) {
		printf("%10s\t", it.table->columns[i].name);
	}
	printf("\n");
}

void print_column_data(struct Iterator it)
{
	if(it.query_status == QUERYSTATUS_INVALID_QUERY)
		return;

	for(size_t i=0; i < it.table->number_of_columns; i++) {
		struct Column* column = &(it.table->columns[i]);
		print_column( get_ptr_column_impl(column, it.row), column->type );
	}
	printf("\n");
}

int repl(struct Database_Handle dbh)
{
	char buffer[100] = {0};
	printf("> ");
	fgets(buffer, sizeof(buffer) - 1, stdin);

	char * name = calloc(sizeof(char), 100);
	sscanf(buffer, "%s", name);
	
	if(strcmp(name, "") == 0)
		return 0;

	struct Iterator it = prepare_query(dbh, name, 0);

	print_column_headers(it);
	while( iterate(&it) != ITERATE_END )
		print_column_data(it);

	free_iterator(it);

	return 1;
}

