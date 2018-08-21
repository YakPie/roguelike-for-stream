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
	const int rules_length = 2;

	char* input = "s";

//	for(int for_i=0; for_i<2; for_i++) {
		char* output = calloc(sizeof(char), 255);
		strcpy(output, input);
		int output_i = 0;

		int output_len = strlen(output);

		for(
				int input_i = 0;
				input_i < output_len && input_i < 255;
				input_i++
			) {
			for(int i = 0; i < rules_length; i++) {
				if( output[input_i] == rules[i].id ) {
					strcpy(&output[output_i], rules[i].replace);
					output_i += strlen(rules[i].replace);
				} else {
					if(output_i != input_i)
						output[output_i] = output[input_i];
					output_i++;
				}
			}
			//sleep(1);
			//printf("still going\n");
		}
		printf("%s\n", output);
//	}

	printf("We finnished gennerating the map\n");

	int c;
	while(c = getchar())
		printf("%c\n", c);

	return 0;
}
