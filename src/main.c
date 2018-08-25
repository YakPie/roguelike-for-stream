#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

struct Rules {
	char id;
	char* replace;
};

struct Point {
	int x;
	int y;
};

struct LinkedList {
	struct Point point;
	struct LinkedList* next;
};

struct Point pop(struct LinkedList* root) {
	struct LinkedList* current = root;
	struct LinkedList* past = root;
	while(current->next != NULL) {
		past = current;
		current = current->next;
	}

	struct Point ret = current->point;
	free(current);
	past->next = NULL;

	return ret;
}

void push(struct LinkedList* root, struct Point point) {
	struct LinkedList* current = root;
	while(current->next != NULL)
		current = current->next;

	current->next = malloc(sizeof(struct LinkedList));
	current->next->point = point;
	current->next->next = NULL;
}

void debugRuleArray(struct Rules* rules, int rules_length)
{
	printf("Rules: \n");
	for(int i = 0; i < rules_length; i++) {
		printf("Rule %c => %s\n", rules[i].id, rules[i].replace);
	}
}

char* replace(struct Rules* rules, int rules_length, char check)
{
	// Shuffle up the rules array
	for(int i = 0; i < rules_length; i++) {
		size_t random = rand() % rules_length;
		struct Rules tmp = rules[random];
		rules[random] = rules[i];
		rules[i] = tmp;
	}

	for(int i = 0; i < rules_length; i++) {
		if( check == rules[i].id ) {
			return rules[i].replace;
		}
	}
	return NULL; // No match
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
	 *  b = backtrack (maybe in the future?)
	 *  p = push rules (pushes the current position to the stack)
	 *  o = pop rules (pops position of the stack)
	 */
	struct Rules rules[] = {
//		{ .id = 'f',  .replace = "s" },
//		{ .id = 's',  .replace = "sl" },
//		{ .id = 's',  .replace = "sr" },
//		{ .id = 's',  .replace = "sf" },
//		{ .id = 'l',  .replace = "ll" },
//		{ .id = 'l',  .replace = "llplfffrror" },
//		{ .id = 's',  .replace = "spllllfffffffo" },
//		{ .id = 's',  .replace = "sprrrrrro" },
	};
	char* starting_rules = "sfpllffpfforrrorrffe";
	int num_replacements = 1;
	const int rules_length = sizeof(rules)/sizeof(*rules);

	while(true) {
		const int buffer_size = 1024 * 10;
		char* output = calloc(sizeof(char), buffer_size);
		char* input  = calloc(sizeof(char), buffer_size);
		strcpy(input, starting_rules);

		// Replacement rules applying
		for(int for_i=0; for_i<num_replacements; for_i++) {
			strcpy(output, input);
			int output_i = 0;
			int output_len = strlen(output);

			for(
					int input_i = 0;
					input_i < output_len && input_i < buffer_size;
					input_i++
				) {
		
				char* ret = replace(
						rules,
						rules_length,
						input[input_i]
					);

				if(ret != NULL) { // found a replacement
					// copy the new replacement into the output
					strcpy(&output[output_i], ret);
					output_i += strlen(ret);
				} else {
					// copy the input to the output, no change happens
					output[output_i] = input[input_i];
					output_i++;
				}
			}
			memcpy(input, output, buffer_size);
		}
		free(input);

		// Setup ncurses
		initscr();
		int row, col;
		getmaxyx(stdscr, row, col);
		mvprintw(1, 10, "Dungeon overview:");
		mvprintw(2, 10, "Hit a random key to get a new dungeon");
		mvprintw(3, 10, "Dungeon rule:");
		mvprintw(3, 25, output);

		struct Point pos = {
			.x = (row - 5) / 2,
			.y = col / 4
		};

		struct Point tmp_pos = pos;
		struct LinkedList* stack = malloc(sizeof(struct LinkedList));
		stack->point = tmp_pos;
		stack->next = NULL; 

		for(int i=0; i < strlen(output); i++) {
			switch(output[i])
			{
				case 's':
					break;
				case 'l':
					pos.x--;
					break;
				case 'r':
					pos.x++;
					break;
				case 'f':
				case 'e':
					pos.y--;
					break;
				case 'p': // pushing
					tmp_pos.x = pos.x;
					tmp_pos.y = pos.y;
					push(stack, tmp_pos);
					break;
				case 'o': // pop
					tmp_pos = pop(stack);
					pos.x = tmp_pos.x;
					pos.y = tmp_pos.y;
					break;
			}
			char* room_name = calloc(sizeof(char), 2);
			room_name[0] = output[i];
			// add_room( pos );
			// add_edges( prev_pos, pos );
			mvprintw(pos.y*2, pos.x*2, room_name);
		}

		refresh();
		getch();
		clear();
		endwin();

		free(output);
	}

	return 0;
}
