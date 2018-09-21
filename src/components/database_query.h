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

// SELECT QUERY EXAMPLE:
//    position(entityId, x, y), health(entityId, >0)?
// INSERT QUERY EXAMPLE:
//    position(10, 5, 15).
// DELETE QUERY EXAMPE:
// 	delete position(10, 5, 15).
enum YGQL_TokenType {
	YGQL_TOKENTYPE_NAME,           // [a-zA-Z][a-zA-Z0-9_]*
	YGQL_TOKENTYPE_OPEN_PARENT,    // (
	YGQL_TOKENTYPE_CLOSE_PARENT,   // )
	YGQL_TOKENTYPE_SEPERATOR,      // ,
	YGQL_TOKENTYPE_QUESTIONMARK,   // ?
	YGQL_TOKENTYPE_ENDSTATEMENT,   // .
//	YGQL_TOKENTYPE_DELETE,         // delete
	YGQL_TOKENTYPE_UNKOWN,         // 
};

struct YGQL_Token {
	enum YGQL_TokenType type;
};

struct YGQL_AST {

};

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

//char* compile(struct AST*)

#endif