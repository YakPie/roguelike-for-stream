#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Rules {
	char id;
	char* replace;
};

char* replace(
		struct Rules* rules,
		int rules_length,
		char check
		)
{
	for(int i = 0; i < rules_length; i++) {
		if( check == rules[i].id ) {
			return rules[i].replace;
		}
	}
	return NULL; // No match
}

int main(void)
{
	struct Rules rules[] = {
		{ .id = 's',  .replace = "ea" },
		{ .id = 'e',  .replace = "a" }
	};
	const int rules_length = 2;

	const int buffer_size = 1024;
	char* output = calloc(sizeof(char), buffer_size);
	char* input  = calloc(sizeof(char), buffer_size);
	strcpy(input, "s");

	for(int for_i=0; for_i<2; for_i++) {
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

		printf("%s\n", output);
		memcpy(input, output, buffer_size);
	}

	free(output);
	free(input);

	return 0;
}
