#include "database/query.h"
#include "database/core.h"

int main()
{
	{
		struct Query q = parse_query("position.schema");
		printf("Table_name: %s schema: %d\n", q.table_name, q.query_schema);
	}
	{
		struct Query q = parse_query("position");
		printf("Table_name: %s schema: %d\n", q.table_name, q.query_schema);
	}
	{
		struct Query q = parse_query("table_with_char");
		printf("Table_name: %s schema: %d\n", q.table_name, q.query_schema);
	}
	return 0;
}
