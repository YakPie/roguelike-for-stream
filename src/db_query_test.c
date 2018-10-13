#include "database/query.h"
#include "database/core.h"

int main()
{
	/*
	struct YGQL_Token token = ygql_scanner("posistion");
	char * token_desc = ygql_token_descriptor(token.type); 
	printf("Token: %s\n", token_desc);
	printf("Token data: %s\n", token.data);
	//*/
	
	struct Query q = parse_query("position");
	printf("%s\n", q.table_name);


	return 0;
}
