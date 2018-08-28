#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ncurses.h>
#undef KEY_ENTER

#include "components/components.h"
#include "systems/pcg_dungeon.h"
#include "systems/rendering_ncurses.h"

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
};

struct RulesWrapper {
	struct Rules* rules;
	int number_of_rules;
};

struct Token scanner( FILE* fd ) {
	struct Token tk;
	tk.type = TOKENTYPE_UNKNOWN;
	char buffer[100];
	// TODO: needs to ignore spaces and tabs
	// (non-newline whitespace characters)

	int ret = fscanf( fd, "%[a-zA-Z0-9]", buffer);
	if(ret == 1) {
		tk.type = TOKENTYPE_ID;
		tk.data = calloc(sizeof(char), 2);
		tk.data[0] = buffer[0];
		return tk;
	}
	else if(ret > 1) {
		tk.type = TOKENTYPE_REPLACE;
		tk.data = calloc(sizeof(char), ret+1);
		strncpy(tk.data, buffer, ret+1);
		return tk;
	}
	
	ret = fscanf( fd, "%*[ ]=>");
	// check if succesfull, return TOKENTYPE_ARROW
	
	// need to check for EOF
	
	// need to parse comments
	
	// need to parse newlines
//	int c = fgetc( fd );
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
	struct Rules rules[max_rules];
	int number_of_rules = 0;

	cur = scanner(fd); 
	while(cur.type != TOKENTYPE_END_OF_FILE) {
		switch(cur.type) {
			case TOKENTYPE_COMMENT:
				cur = scanner(fd);
				// Intentionally not breaking
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

struct Token expected_token(struct Token cur, enum TokenType type) {
	if(cur.type != type) {
		char *token_str;
		switch(type) {
			case TOKENTYPE_ID:
				token_str = "TOKENTYPE_ID";
				break;
			case TOKENTYPE_ARROW:
				token_str = "TOKENTYPE_ARROW";
				break;
			case TOKENTYPE_REPLACE:
				token_str = "TOKENTYPE_REPLACE";
				break;
			case TOKENTYPE_COMMENT:
				token_str = "TOKENTYPE_COMMENT";
				break;
			case TOKENTYPE_NEWLINE:
				token_str = "TOKENTYPE_NEWLINE";
				break;
			case TOKENTYPE_END_OF_FILE:
				token_str = "TOKENTYPE_END_OF_FILE";
				break;
			default:
				token_str = "UNKNOWN_TOKEN";
				break;
		}
		fprintf(stderr, "Unexpected token, expected %s", token_str);
		exit(SOMETHING_BROKE);
	}
	return cur;
}

struct Rules parse_rule(struct Token cur, FILE* fd) {
	struct Rules rule;

	expected_token(cur, TOKENTYPE_ID);
	rule.id = *cur.data;
	free(cur.data);

	expected_token(scanner(fd), TOKENTYPE_ARROW);
	// TODO: accept both REPLACE and ID
	cur = expected_token(scanner(fd), TOKENTYPE_REPLACE);
	rule.replace = cur.data;

	cur = scanner(fd);
	if(cur.type == TOKENTYPE_COMMENT) {
		cur = scanner(fd);
	}
	expected_token(scanner(fd), TOKENTYPE_NEWLINE);
	
	return rule;
}

struct RulesWrapper parser( FILE* fd ) {
	return parse_level(fd);
}

int main(void)
{
	srand(time(NULL));

	/*
	 *  s = start of the level / entry
	 *  e = end of the level
	 *  l = moved left and create a room
	 *  r = move right and create a room
	 *  f = move upward/forward and create a room
	 *  p = push rules (pushes the current position to the stack)
	 *  o = pop rules (pops position of the stack)
	 *  q = turn clockwise
	 *  a = turn anti-clockwise
	 *  0 = reset direction
	 *  1 = slime
	 *  2 = goblin
	 *
	struct Rules rules[] = {
//		{ .id = 'f',  .replace = "s" },
		{ .id = 's',  .replace = "sl" },
		{ .id = 's',  .replace = "sr" },
		{ .id = 's',  .replace = "sf" },
		{ .id = 'l',  .replace = "l1" },
		{ .id = 'l',  .replace = "l" },
		{ .id = 'r',  .replace = "r2" },
		{ .id = 'r',  .replace = "r" },
//		{ .id = 'l',  .replace = "ll" },
//		{ .id = 'l',  .replace = "l" },
//		{ .id = 'l',  .replace = "llplfffrror" },
		{ .id = 's',  .replace = "spllffffrro" },
//		{ .id = 's',  .replace = "sprrrrrro" },
		{ .id = 's',  .replace = "safqfqf" },
	};
//	char* starting_rules = "sfpllffpfforrrorrffe";
	*/

	FILE *fd = fopen("./level1", "r");
	struct RulesWrapper rw = parser(fd);
	fclose(fd);

	char* starting_rules = "se";
	int num_replacements = 10;
//	const int rules_length = sizeof(rules)/sizeof(*rules);

	// Setup ncurses
	initscr();
//	setlocale(LC_ALL,"C-UTF-8");
	noecho();

	int ch;
	char* output = NULL;
	struct Graph* dag = NULL;

	const int KEY_D = 100;
	const int KEY_W = 119;
	const int KEY_A = 97;
	const int KEY_S = 115;
	const int KEY_ENTER = 10;

	int x = 0,
		 y = 0;

	while(true) {
		// Prints some debug info
		mvprintw(1, 10, "Dungeon");
		mvprintw(3, 10, "Debug rule:");
		
		if(output == NULL || dag == NULL || ch == KEY_ENTER) {
			if(output != NULL) free(output);
			if(dag != NULL) free(dag);

			output = rule_engine(
				rw.rules,
				starting_rules,
				num_replacements,
				rw.number_of_rules
			); 

			// Travel the dungeon rule and gennerates a DAG
			dag = create_dag_from_dungeonrule(output);

			for(int i=0; i<dag->number_of_nodes; i++) {
				if(dag->nodes[i].type == 's') {
					x = dag->nodes[i].position.x;
					y = dag->nodes[i].position.y;
					break;
				}
			}
		}
		mvprintw(2, 10, "The current position is x %d and y %d", x, y);

		mvprintw(3, 25, output);
		print_room_ncurses(dag);
		print_room_info(dag, x, y);

		// Wait on character
		ch = getch();
		clear();

		switch(ch) {
			case KEY_W: 
				mvprintw(5, 10, "KEY_UP!");
				y--;
				break;
			case KEY_S: 
				mvprintw(5, 10, "KEY_DOWN!");
				y++;
				break;
			case KEY_A: 
				mvprintw(5, 10, "KEY_LEFT!");
				x--;
				break;
			case KEY_D: 
				mvprintw(5, 10, "KEY_RIGHT!");
				x++;
				break;
			default:
				mvprintw(5, 10, "%d", ch);
		}
		
	}
	
	free(output);
	free(dag);

	endwin();

	return 0;
}
