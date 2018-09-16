#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H 

/*
 *  table Position {
 *		size_t entityId,
 *		int x,
 *		int y
 *  }
 *
 *  table Health {
 *		size_t entityId,
 *		float hp; // [0,1]
 *  }
 *
 *  Query for healthy entity (datalog-like syntax):
 *	 	position(entityId, x, y), health(entityId, >0)?
 *
 *	
 *	  Char* query
 *	      |          Parsed
 *	      \/
 *	  struct AST*
 *	      |          Compiled
 *	      \/
 *	 char* bytecode
 *	      |          Executed
 *       \/
 *  Database result
 */

// Yakpie's Game Query Language (YGQL)

#include <stdlib.h>

struct YGQL_Token {

};

struct YGQL_AST {

};

struct YGQL_Token scanner(char* query) {
	struct YGQL_Token token = {

	};

	return token;
}

struct YGQL_AST* parse(char* query) {
	struct YGQL_AST* ast = calloc(sizeof(struct YGQL_AST), 1);
	return ast;
}

//char* compile(struct AST*)

#endif
