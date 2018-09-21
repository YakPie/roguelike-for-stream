#include "database/query.h"
#include "database/vm.h"
#include "database/core.h"

int main()
{
	struct YGQL_AST* ast = ygql_parse("position(10, 5, 15).");
 
	printf("Test\n");
	return 0;
}
