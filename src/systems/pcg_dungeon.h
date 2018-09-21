#ifndef PCG_DUNGEON_H 
#define PCG_DUNGEON_H 

#include "../components/components.h"

char* replace(struct Rules* rules, int rules_length, char check);

char* rule_engine(
		struct Rules* rules,
		char* starting_rules, 
		int num_replacements, 
		const int rules_length
);

struct Graph* create_dag_from_dungeonrule(char *output);

#endif
