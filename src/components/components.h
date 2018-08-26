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
	struct Point position;
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

enum ReturnCodes {
	ALL_GOOD = 0,
	SOMETHING_BROKE = 1
};

struct Graph {
	struct Room nodes[1024] ; // this should be an array
	struct Edge edges[1024*4]; // this should be an array
	unsigned int number_of_nodes;
	unsigned int number_of_edges;
};

int add_room( struct Graph* dag, struct Point pos, char type ) {
	if(dag->number_of_nodes == 1024)
		return SOMETHING_BROKE;

	// check that the room doesn't allready exist
	for(int i=0; i < dag->number_of_nodes; i++) {
		if(
			dag->nodes[i].position.x == pos.x && 
			dag->nodes[i].position.y == pos.y
		)
			return ALL_GOOD;
	}

	struct Room room = {
		.position = pos,
		.type = type
	};
	dag->nodes[dag->number_of_nodes] = room;
	dag->number_of_nodes++;

	return ALL_GOOD;
}

int add_edges(
		struct Graph* dag,
		struct Point from,
		struct Point to 
) {
	if(dag->number_of_edges == 1024*4)
		return SOMETHING_BROKE;

	if( from.x == to.x && from.y == to.y )
		return ALL_GOOD;

	// check that the edge doesn't allready exist
	for(int i=0; i < dag->number_of_edges; i++) {
		if(
			dag->edges[i].from.x == from.x && 
			dag->edges[i].from.y == from.y &&
			dag->edges[i].to.x == to.x && 
			dag->edges[i].to.y == to.y
		)
			return ALL_GOOD;
	}

 	struct Edge edge = {
		.from = from,
		.to = to
	};
	dag->edges[dag->number_of_edges] = edge;
	dag->number_of_edges++;

	return ALL_GOOD;
}

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
