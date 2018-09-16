#include "components/database_query.h"
#include "components/database_vm.h"
#include "components/database.h"

int main()
{
	struct YGQL_AST* ast = ygql_parse("position(10, 5, 15).");
 
	printf("Test\n");
	return 0;
}
