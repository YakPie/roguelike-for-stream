#ifndef COMPONENTS_H
#define COMPONENTS_H

struct Rules {
	char id;
	char* replace;
};

struct Point {
	int x;
	int y;
};

struct LinkedList {
	struct Point point; // To make this generic, we might have void*
	struct LinkedList* next;
};

struct Room {
	struct Point point;
	char type;
};

struct Edge {
	struct Point from;
	struct Point to;
};

enum Direction {
	UP = 0,
	RIGHT,
	DOWN,
	LEFT
};

struct Vec {
	// So much memory is allocated
	size_t current_capacity; 
	// Size of whatever is kept in the vector
	size_t size; 
	// realloc, how much more memory do you need
	size_t allocated_gap; 
	// amable1408: You even make a union to use stack allocation for the first N bytes and then if it's higher use heap
	void * data;
};

struct Graph {
	struct Room nodes; // this should be an array
	struct Edge edges; // this should be an array
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



#endif
