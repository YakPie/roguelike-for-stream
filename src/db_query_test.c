#include "database/query.h"
#include "database/core.h"

int main()
{
	struct Query q = parse_query("position");
	return 0;
}
