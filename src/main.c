#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Rules {
	char id;
	char* replace;
};

int main(void)
{
	printf("Welcome to my awesome game\n");
	printf("Its some roguelike thingy\n");

	struct Rules rules[] = {
		{ .id = 's',  .replace = "ea" },
		{ .id = 'e',  .replace = "a" }
	};
	const int rules_length = 1;

	char* input = "s";

	for(int for_i=0; for_i<2; for_i++) {
		char* output = calloc(sizeof(char), 255);
		int output_i = 0;

		for(int input_i = 0; input_i < strlen(input); input_i++) {
			for(int i = 0; i < rules_length; i++) {
				if( input[input_i] == rules[i].id ) {
					strcpy(&output[output_i], rules[i].replace);
					output_i += strlen(rules[i].replace);
				} else {
					strcpy(&output[output_i], &input[input_i]);
					output_i++;
				}
			}
		}
		printf("%s\n", output);
	}

	printf("We finnished gennerating the map\n");

	int c;
	while(c = getchar())
		printf("%c\n", c);

	return 0;
}
