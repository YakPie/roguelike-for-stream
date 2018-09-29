#include "repl.h"
#include <stdio.h>

void print_column(void *it, struct Datatype dt) {
	if(strcmp(dt.name, "integer") == 0) {
		printf("%d\t", *(int *)it);
	} else if(strcmp(dt.name, "uint") == 0) {
		printf("%u\t", *(unsigned int *)it);
	} else if(strcmp(dt.name, "float") == 0) {
		printf("%f\t", *(float *)it);
	} else if(strcmp(dt.name, "char") == 0) {
		printf("%c\t", *(char *)it);
	} else if(strcmp(dt.name, "string") == 0) {
		printf("%s\t", (char *)it);
	}
}

void print_column_headers(struct Iterator it) {
	for(int i=0; i < it.table->number_of_columns; i++) {
		printf("%s\t", it.table->columns[i].name);
	}
	printf("\n");
}

void print_column_data(struct Iterator it)
{
	for(int i=0; i < it.table->number_of_columns; i++) {
		print_column(
				get_ptr_column(it.table, it.row, i),
				it.table->columns[i].type
				);
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
	printf("\"%s\"\n", name);

	if(strcmp(name, "") == 0)
		return 0;

	struct Query query_table = {
		.table_name = name
	};
	struct Iterator it = query(dbh, query_table);

	if(it.found_table != 1) return 1;

	print_column_headers(it);
	printf("---\n");
	do {
		print_column_data(it);
	} while( iterate(&it) != ITERATE_END );

	return 1;
}

