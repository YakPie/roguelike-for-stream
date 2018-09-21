#include "query.h"
#include <stdlib.h>

struct YGQL_Token ygql_scanner(char* query) {
	struct YGQL_Token token = {
		.type = YGQL_TOKENTYPE_UNKOWN 
	};

	// scanning for token types

	return token;
}

//
struct YGQL_AST* ygql_parse(char* query) {
	struct YGQL_AST* ast = calloc(sizeof(struct YGQL_AST), 1);



	return ast;
}
