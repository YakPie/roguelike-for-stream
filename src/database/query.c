#include "query.h"
#include <stdlib.h>
#include <stdio.h>

struct YGQL_Token ygql_scanner(char* query) {
	struct YGQL_Token token = {
		.type = YGQL_TOKENTYPE_UNKOWN 
	};
	char buffer[255];

	int ret = sscanf( query, " %[a-zA-Z0-9]", buffer);
	if(ret == 1)
	{
		token.type = YGQL_TOKENTYPE_NAME;
		token.data[0] = buffer[0];
		return token;
	}

	return token;
}

char* ygql_token_descriptor(enum YGQL_TokenType type) {
	switch(type) {
		case YGQL_TOKENTYPE_NAME:
			return "TOKENTYPE_NAME";
		case YGQL_TOKENTYPE_UNKOWN:
			return "TOKENTYPE_UNKNOWN";
		default:
			return "UNKNOWN_TOKEN";
	}
}

struct YGQL_Token ygql_expected_token(
		struct YGQL_Token cur, enum YGQL_TokenType type, enum YGQL_TokenType type2) 
{ 
	if(cur.type == type || cur.type == type2)
		return cur;

	char *got = ygql_token_descriptor(cur.type);
	char *expected1 = ygql_token_descriptor(type);
	char *expected2 = ygql_token_descriptor(type2);
	fprintf(
		stderr,
		"Unexpected token, got %s but expected %s or %s\n",
		got,
		expected1,
		expected2
	);
	exit(1);
}

struct YGQL_AST* ygql_parse(char* query) {
	struct YGQL_AST* ast = calloc(sizeof(struct YGQL_AST), 1);
	struct YGQL_Token token = {}; 

	token = ygql_expected_token(ygql_scanner(query), YGQL_TOKENTYPE_NAME, YGQL_TOKENTYPE_NAME);
	return ast;
}
