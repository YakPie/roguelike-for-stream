#include <stdlib.h>
#include <stdio.h>
#include "components.h"

int add_room( struct Graph* dag, struct Point pos, char type ) {
	if(dag->number_of_nodes == 1024)
		return SOMETHING_BROKE;

	// check that the room doesn't allready exist
	for(size_t i=0; i < dag->number_of_nodes; i++) {
		if(
			dag->nodes[i].position.x == pos.x && 
			dag->nodes[i].position.y == pos.y
		)
			return ALL_GOOD;
	}

	struct Room room = {
		.position = pos,
		.type = type,
		.number_of_monsters = 0
	};
	dag->nodes[dag->number_of_nodes] = room;
	dag->number_of_nodes++;

	return ALL_GOOD;
}

int add_monster_to_room(
		struct Graph* dag,
		struct Point pos,
		char type
) {
	
	for(size_t i=0; i<dag->number_of_nodes; i++) {
		struct Room* room = &dag->nodes[i];
		if(room->position.x == pos.x &&
			room->position.y == pos.y)
		{
			int monster_type;
			switch(type) {
				case '1':
					monster_type = MONSTER_SLIME;
					break;
				case '2':
					monster_type = MONSTER_GOBLIN;
					break;
				default:
					return SOMETHING_BROKE;
			}
	
			struct Monster monster = {
				.type = monster_type
			};
			room->monsters[room->number_of_monsters] = monster;
			room->number_of_monsters++;

			return ALL_GOOD;
		}
	}

	return SOMETHING_BROKE;
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
	for(size_t i=0; i < dag->number_of_edges; i++) {
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

