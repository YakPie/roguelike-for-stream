#include "query.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
	BUFFER_SIZE = 255
};

struct YGQL_Token ygql_scanner(char* query) {
	struct YGQL_Token token = {
		.type = YGQL_TOKENTYPE_UNKOWN ,
		.data = calloc(BUFFER_SIZE, sizeof(char))
	};

	/*
	switch(query[0]) {
		case '.':
			token.type = YGQL_TOKENTYPE_DOT;
			return token;
	}
	*/

	int ret = sscanf( query, "%s", token.data );
	if(ret == 1)
	{
		token.type = YGQL_TOKENTYPE_NAME;
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

struct YGQL_Token ygql_expected_token(struct YGQL_Token cur, enum YGQL_TokenType type) 
{ 
	if(cur.type == type)
		return cur;

	char *got = ygql_token_descriptor(cur.type);
	char *expected1 = ygql_token_descriptor(type);
	fprintf(
		stderr,
		"Unexpected token, got %s but expected %s\n",
		got,
		expected1
	);
	exit(1);
}

struct Query parse_query(char const * const const_query) {
	size_t len = strlen(const_query) + 1;
	char* query = calloc(len, sizeof(char));
	char* query_orgin = query;
	strncpy(query, const_query, len);

	struct YGQL_Token token = ygql_expected_token(
		ygql_scanner(query),
		YGQL_TOKENTYPE_NAME
	);

	struct Query q = {
		.table_name = token.data
	};

	/*
	assert(query <= query_orgin + len);
	ygql_expected_token(ygql_scanner(query), YGQL_TOKENTYPE_DOT);

	assert(query <= query_orgin + len);
	ygql_expected_token(ygql_scanner(query), YGQL_TOKENTYPE_NAME);
	*/

	free(query_orgin);
	return q;
}
