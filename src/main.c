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
	int line_number;
	int col;
};

struct RulesWrapper {
	struct Rules* rules;
	int number_of_rules;
};

struct Token scanner( FILE* fd ) {
//	printf("ftell: %ld\n", ftell(fd));

	struct Token tk;
	tk.type = TOKENTYPE_UNKNOWN;
	char buffer[100];
	
beginning:
	switch (fgetc(fd)) {
		case ' ': goto beginning;
		case '\t': goto beginning;
		case '\n': 
			tk.type = TOKENTYPE_NEWLINE;
			printf("Returning TOKEN NEWLINE\n");
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
			printf("Returning TOKEN EOF\n");
			return tk;
		default: fseek(fd, -1, SEEK_CUR);
	}

	int ret = fscanf( fd, " %[a-zA-Z0-9]", buffer);

	if(ret == 1) {
		if(strlen(buffer) == 1) {
			tk.type = TOKENTYPE_ID;
			tk.data = calloc(sizeof(char), 2);
			tk.data[0] = buffer[0];
			printf("Returning TOKEN ID\n");
			return tk;
		}
		else if(strlen(buffer) > 1) {
			tk.type = TOKENTYPE_REPLACE;
			tk.data = calloc(sizeof(char), ret+1);
			strncpy(tk.data, buffer, ret+1);
			printf("Returning TOKEN REPLACE\n");
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
		printf("Returning TOKEN ARROW\n");
		return tk;
	}
	
	// need to parse comments
	
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

struct Token expected_token(struct Token cur, enum TokenType type) {
	if(cur.type != type) {
		char *got = token_descriptor(cur.type);
		char *expected = token_descriptor(type);
		fprintf(
			stderr,
			"Unexpected token, got %s but expected %s\n",
			got,
			expected
		);
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
		
		mvprintw(5, 40, "id replace\n");
		for(int i=0; i < rw.number_of_rules; i++) {
			mvprintw(6+i, 40, "%c  %s\n", rw.rules[i].id, rw.rules[i].replace);
		}

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
