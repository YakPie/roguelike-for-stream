#include "database/query.h"
#include "database/vm.h"
#include "database/core.h"

int main()
{
	struct Database_Handle dbh = new_database();

	create_table(dbh, "test", 0);	
	create_table(dbh, "test2", 0);

	struct Column column1 = {
		.name = "Column1",
		.type = datatype_integer,
		.count = 1
	};
	struct Column column2 = {
		.name = "Column2",
		.type = datatype_float,
		.count = 1
	};

	create_table(
		dbh,
		"table_with_columns",
		2, column1, column2
	);

	struct Query test_query = {
		.table_name = "tables"
	};
	query(dbh, test_query);

	printf("\n");

	struct Query test_query2 = {
		.table_name = "table_with_columns",
		.query_schema = 1
	};
	query(dbh, test_query2);

	// Inserting data into the database
	{
		int column1_data_data = 20;
		float column2_data_data = 1.5;
		struct InsertData column1_data = {
			.name = "Column1",
			.data = &column1_data_data
		};
		struct InsertData column2_data = {
			.name = "Column2",
			.data = &column2_data_data
		};
		insert_into(dbh, "table_with_columns", 2,
				column1_data, column2_data);
	}

	// Inserting data into the database
	{
		int column1_data_data = 50;
		float column2_data_data = 2.5;
		struct InsertData column1_data = {
			.name = "Column1",
			.data = &column1_data_data
		};
		struct InsertData column2_data = {
			.name = "Column2",
			.data = &column2_data_data
		};
		insert_into(dbh, "table_with_columns", 2,
				column1_data, column2_data);
	}

	// Query data back
	printf("\n");
	struct Query test_query3 = {
		.table_name = "table_with_columns"
	};
	struct Iterator it = query(dbh, test_query3);

	// Printing out column headers
	print_column_headers(it);

	// Printing out data
	do
	{
		print_column_data(it);
	}
	while(iterate(&it) != ITERATE_END);

	struct Column column_string = {
		.name = "StringStuff",
		.type = datatype_string,
		.count = 255
	};
	create_table(dbh, "table_with_char", 1, column_string);
	struct InsertData column_StringStuff = {
		.name = "StringStuff",
		.data = "test" 
	};
	insert_into(
			dbh, "table_with_char", 1, column_StringStuff);
	struct Query test_query4 = {
		.table_name = "table_with_char"
	};
	printf("\n");
	it = query(dbh, test_query4);

	// Printing out column headers
	print_column_headers(it);

	// Printing out data
	{
		do
		{
			print_column_data(it);
		}
		while(iterate(&it) != ITERATE_END);
	}

	return 0;
}
