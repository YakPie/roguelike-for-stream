#include "query.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
	BUFFER_SIZE = 255
};

struct YGQL_Token ygql_scanner(char* query) {
	struct YGQL_Token token = {
		.type = YGQL_TOKENTYPE_UNKOWN 
	};
	char buffer[BUFFER_SIZE];

	int ret = sscanf( query, "%s", buffer);
	if(ret == 1)
	{
		token.type = YGQL_TOKENTYPE_NAME;

		size_t len = strlen(buffer) + 1;
		token.data = calloc(len, sizeof(char));
		strncpy(token.data, buffer, BUFFER_SIZE);

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

struct Query parse_query(char const * const const_query) {
	size_t len = strlen(const_query) + 1;
	char* query = calloc(len, sizeof(char));
	void * query_orgin = query;
	strncpy(query, const_query, len);

	struct YGQL_Token token = {0}; 

	token = ygql_expected_token(
			ygql_scanner(query), YGQL_TOKENTYPE_NAME, YGQL_TOKENTYPE_NAME);

	struct Query q = {
		.table_name = token.data
	};

	free(query_orgin);
	return q;
}
