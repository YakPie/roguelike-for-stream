#ifndef PCG_DUNGEON_H 
#define PCG_DUNGEON_H 

#include "../components/components.h"

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

char* rule_engine(
		struct Rules* rules,
		char* starting_rules, 
		int num_replacements, 
		const int rules_length
)
{
	const int buffer_size = 1024;
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
	return output;
}

struct Graph* create_dag_from_dungeonrule(char *output)
{
	struct Point pos = {
		.x = 0,
		.y = 0
	};

	struct Point tmp_pos = pos;
	struct Point prev_pos = pos;
	struct LinkedList* stack = malloc(sizeof(struct LinkedList));
	stack->point = tmp_pos;
	stack->next = NULL; 

	int direction = 0;
	struct Graph* dag = calloc(sizeof(struct Graph), 1);

	for(int i=0; i < strlen(output); i++) {
		prev_pos = pos;

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
				switch(direction) {
					case UP:
						pos.y--;
						break;
					case LEFT:
						pos.x--;
						break;
					case RIGHT:
						pos.x++;
						break;
					case DOWN:
						pos.y++;
						break;
				}
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
			case 'q': // clockwise
				direction = (direction+1)%4;
				break;
			case 'a': // anti-clockwise
				direction = (direction+4-1)%4;
				break;
			case '0':
				direction = UP;
				break;
		}

		add_room( dag, pos, output[i] );
		add_edges( dag, prev_pos, pos );
	}

	return dag;
}



#endif
