#include "repl.h"
#include <stdio.h>

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
