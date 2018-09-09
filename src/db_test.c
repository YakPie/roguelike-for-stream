#include "components/database.h"

int main()
{
	struct Database_Handle dbh = new_database();

	create_table(dbh, "test");	
	create_table(dbh, "test2");	
	create_table(dbh, "test3");	

	struct Query test_query = {
		.table_name = "tables"
	};
	query(dbh, test_query);

	return 0;
}
