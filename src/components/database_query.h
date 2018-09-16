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
 *  Query for healthy entity:
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

#endif
