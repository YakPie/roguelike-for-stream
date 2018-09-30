#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdlib.h>
#include <stdio.h>

enum GameState
{
	GAMESTATE_END,
	GAMESTATE_MAP,
	GAMESTATE_ROOM,
	GAMESTATE_REPL
};

struct Rules {
	char id;
	char* replace;
};

struct Point {
	int x;
	int y;
};

enum MonsterTypes {
	MONSTER_SLIME,
	MONSTER_GOBLIN
	// lapisftw suggested that yakpie should be a monster type
	// is it a weak monster? or a big boss monster?
};

struct Monster {
	int type; // MonsterTypes
	// Damage dealt
	// Char representation
	// Health
	// Modifier - like Huge or beafly or champion
	// Iniative / attack speed
	// Position
	struct Point position;
};

struct Room {
	struct Point position;
	char type;
	// List monsters
	struct Monster monsters[10]; 
	int number_of_monsters;
	// List treasures
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

// TODO: refactor game level Graph into a database table
struct Graph {
	struct Room nodes[1024] ;
	struct Edge edges[1024*4];
	unsigned int number_of_nodes;
	unsigned int number_of_edges;
};

// TODO: refactor rules into into a Rule database table
struct RulesWrapper {
	struct Rules* rules;
	int number_of_rules;
};

int add_room( struct Graph* dag, struct Point pos, char type );
int add_monster_to_room(struct Graph* dag, struct Point pos, char type);
int add_edges(struct Graph* dag, struct Point from, struct Point to );

#endif
