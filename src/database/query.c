#include "query.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum {
	BUFFER_SIZE = 255
};

struct YGQL_Token ygql_scanner(struct ParserIt* it) {
	struct YGQL_Token token = {
		.type = YGQL_TOKENTYPE_UNKOWN ,
		.data = calloc(BUFFER_SIZE, sizeof(char))
	};
	char const * const query = it->query + it->offset;

	switch(query[0]) {
		case '.':
			token.type = YGQL_TOKENTYPE_DOT;
			it->offset += 1;
			return token;
		case '\0':
			token.type = YGQL_TOKENTYPE_EOL;
			return token;
	}

	int ret = sscanf( query, "%[a-zA-Z0-9_]", token.data );
	if(ret == 1)
	{
		token.type = YGQL_TOKENTYPE_NAME;
		it->offset += strlen(token.data);
		return token;
	}

	return token;
}

char* ygql_token_descriptor(enum YGQL_TokenType type) {
	switch(type) {
		case YGQL_TOKENTYPE_NAME:
			return "TOKENTYPE_NAME";
		case YGQL_TOKENTYPE_DOT:
			return "TOKENTYPE_DOT";
		case YGQL_TOKENTYPE_UNKOWN:
			return "TOKENTYPE_UNKNOWN";
		case YGQL_TOKENTYPE_EOL:
			return "TOKENTYPE_EOL";
	}

	return "UNKNOWN_TOKEN";
}

static struct YGQL_Token ygql_expected_token(struct YGQL_Token cur, enum YGQL_TokenType type) 
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

struct Query parse_query(char const * const const_query)
{
	struct ParserIt it = {
		.query = const_query,
		.offset = 0
	};

	struct YGQL_Token token = ygql_expected_token(
		ygql_scanner(&it),
		YGQL_TOKENTYPE_NAME
	);

	struct Query q = {
		.table_name = token.data
	};

	token = ygql_scanner(&it);
	if(token.type == YGQL_TOKENTYPE_EOL)
		return q;

	ygql_expected_token(token, YGQL_TOKENTYPE_DOT);
	token = ygql_expected_token(ygql_scanner(&it), YGQL_TOKENTYPE_NAME);

	if(strcmp(token.data, "schema") == 0)
		q.query_schema = 1;		

	return q;
}
