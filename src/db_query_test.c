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
	{
		struct Query q = parse_query("table_with_char()");
		printf("Table_name: %s schema: %d\n", q.table_name, q.query_schema);
	}
	{
		struct ParserIt it = {
			.query = "()",
			.offset = 0
		};
		struct YGQL_Token tkn = ygql_scanner(&it);
		printf("offset: %td, %s\n", it.offset, ygql_token_descriptor(tkn.type));
		tkn = ygql_scanner(&it);
		printf("offset: %td, %s\n", it.offset, ygql_token_descriptor(tkn.type));
	}
	{
		struct ParserIt it = {
			.query = "test_table()",
			.offset = 0
		};
		struct YGQL_Token tkn = ygql_scanner(&it);
		printf("offset: %td %c, %s\n", it.offset, *(it.query + it.offset), ygql_token_descriptor(tkn.type));
		tkn = ygql_scanner(&it);
		printf("offset: %td %c, %s\n", it.offset, *(it.query + it.offset), ygql_token_descriptor(tkn.type));
		tkn = ygql_scanner(&it);
		printf("offset: %td %c, %s\n", it.offset, *(it.query + it.offset), ygql_token_descriptor(tkn.type));
	}

	return 0;
}
