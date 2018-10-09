#include "../components/components.h"
#include "grammer_parser.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// File example:
//   
// s => sl
// s => sr # comment support
// s => sf
// l => l1
//
enum TokenType {
	TOKENTYPE_ID,          // ([a-zA-Z0-9])
	TOKENTYPE_REPLACE,     // ([a-zA-Z0-9]+)
	TOKENTYPE_ARROW,       // \=\>
	TOKENTYPE_COMMENT,     // \#.*$
	TOKENTYPE_NEWLINE,     // \n
	TOKENTYPE_END_OF_FILE, // EOF
	TOKENTYPE_UNKNOWN
};

struct Token {
	enum TokenType type;
	char *data;
	int line_number;
	int col;
};

struct Token scanner( FILE* fd ) {
	struct Token tk;
	tk.type = TOKENTYPE_UNKNOWN;
	char buffer[100];
	
beginning:
	switch (fgetc(fd)) {
		case ' ': goto beginning;
		case '\t': goto beginning;
		case '\n': 
			tk.type = TOKENTYPE_NEWLINE;
			return tk;
		case '#': {
			int tmp = fgetc(fd);
			while(tmp != '\n' && tmp != EOF)
				tmp = fgetc(fd);
			tk.type = TOKENTYPE_COMMENT;
			return tk;
		}
		case EOF:
			tk.type = TOKENTYPE_END_OF_FILE;
			return tk;
		default: fseek(fd, -1, SEEK_CUR);
	}

	int ret = fscanf( fd, " %[a-zA-Z0-9]", buffer);

	if(ret == 1) {
		if(strlen(buffer) == 1) {
			tk.type = TOKENTYPE_ID;
			tk.data = calloc(2, sizeof(char));
			tk.data[0] = buffer[0];
			return tk;
		}
		else if(strlen(buffer) > 1) {
			tk.type = TOKENTYPE_REPLACE;
			tk.data = calloc((size_t)ret + 1, sizeof(char));
			strncpy(tk.data, buffer, ret+1);
			return tk;
		}
	}
	
	char *c = calloc(sizeof(char), 3);
	// todo: need to check the return code of fscanf
	// bpaf: do you want to remove the line 72 todo and ignore fscanf? I think you can even pass NULL instead of c and avoid alloc
	ret = fscanf( fd, " %2c", c ); 
	ret = strcmp("=>", c );
	free(c);
	if(ret == 0) {
		tk.type = TOKENTYPE_ARROW;
		return tk;
	}
	
	return tk;
}

// LEVEL => RULE LEVEL
//			| COMMENT? NEWLINE
//			| EOF
//
// RULE  => ID ARROW REPLACE COMMENT? NEWLINE
// Rewrite a struct that also have the length of the array
struct Rules parse_rule(struct Token cur, FILE* fd);
struct RulesWrapper parse_level(FILE* fd) {	
	struct Token cur;
	const int max_rules = 10;
	struct Rules* rules = calloc(sizeof(struct Rules), max_rules);
	int number_of_rules = 0;

	cur = scanner(fd); 
	while(cur.type != TOKENTYPE_END_OF_FILE) {
		switch(cur.type) {
			case TOKENTYPE_COMMENT:
			case TOKENTYPE_NEWLINE:
				break;

			default:
				rules[number_of_rules] = parse_rule(cur, fd);
				number_of_rules++;

				if(number_of_rules > 10) {
					fprintf(stderr, "More than 10 rules\n");
					break;
				}
		}
		cur = scanner(fd); 
	}

	struct RulesWrapper rw = {
		.rules = rules,
		.number_of_rules = number_of_rules
	};
	return rw;
}

char * token_descriptor(enum TokenType type) {
	switch(type) {
		case TOKENTYPE_ID:
			return "TOKENTYPE_ID";
		case TOKENTYPE_ARROW:
			return "TOKENTYPE_ARROW";
		case TOKENTYPE_REPLACE:
			return "TOKENTYPE_REPLACE";
		case TOKENTYPE_COMMENT:
			return "TOKENTYPE_COMMENT";
		case TOKENTYPE_NEWLINE:
			return "TOKENTYPE_NEWLINE";
		case TOKENTYPE_END_OF_FILE:
			return "TOKENTYPE_END_OF_FILE";
		case TOKENTYPE_UNKNOWN:
			return "TOKENTYPE_UNKNOWN";
	}

	return "UNKNOWN_TOKEN";
}

struct Token expected_token(
		struct Token cur,
		enum TokenType type,
		enum TokenType type2)
{ 
	if(cur.type == type || cur.type == type2)
		return cur;

	char *got = token_descriptor(cur.type);
	char *expected1 = token_descriptor(type);
	char *expected2 = token_descriptor(type2);
	fprintf(
		stderr,
		"Unexpected token, got %s but expected %s or %s\n",
		got,
		expected1,
		expected2
	);
	exit(SOMETHING_BROKE);
}

struct Rules parse_rule(struct Token cur, FILE* fd) {
	struct Rules rule;

	expected_token(cur, TOKENTYPE_ID, TOKENTYPE_ID);
	rule.id = *cur.data;
	free(cur.data);

	expected_token(scanner(fd), TOKENTYPE_ARROW, TOKENTYPE_ARROW);
	cur = expected_token(scanner(fd), TOKENTYPE_REPLACE, TOKENTYPE_ID);
	rule.replace = cur.data;
	
	return rule;
}

struct RulesWrapper parser( FILE* fd ) {
	return parse_level(fd);
}
