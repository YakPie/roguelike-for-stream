#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

struct Rules {
	char id;
	char* replace;
};

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
	 *  l = create a room to the left
	 *  r = create a room to the right
	 *  f = create a room forward
	 *  b = backtrack (maybe in the future?)
	 */
	struct Rules rules[] = {
		{ .id = 's',  .replace = "sl" },
		{ .id = 's',  .replace = "sr" },
		{ .id = 's',  .replace = "sf" },
		{ .id = 'l',  .replace = "llffrr" },
	};
	const int rules_length = 3;

	while(true) {
		const int buffer_size = 1024;
		char* output = calloc(sizeof(char), buffer_size);
		char* input  = calloc(sizeof(char), buffer_size);
		strcpy(input, "se");

		for(int for_i=0; for_i<20; for_i++) {
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
		int row, col, x, y;
		getmaxyx(stdscr, row, col);
		y = row / 2;
		x = col / 2;

		for(int i=0; i < strlen(output); i++) {
			switch(output[i])
			{
				case 's':
					break;
				case 'l':
					x--;
					break;
				case 'r':
					x++;
					break;
				case 'f':
				case 'e':
					y--;
					break;
			}
			char* room_name = calloc(sizeof(char), 2);
			room_name[0] = output[i];
			mvprintw(y, x, room_name);
		}

		refresh();
		getch();
		clear();
		endwin();

		free(output);
	}

	return 0;
}
