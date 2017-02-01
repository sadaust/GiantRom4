#pragma once
#include "Creature.h"
#include "Spawner.h"



#define creatureTypes 3
#define numCreatureSprites 4
#define MAXTOSPAWN 100

class CreatureMan {
private:
	std::vector <Creature> creatures;
	std::vector <Spawner> spawners;
	int numtospawn;
	float timebetweenspawns;
	float speedmultiplier;
	spriteStruct creaturesprites[creatureTypes][numCreatureSprites];
public:
	CreatureMan();
	void init();
	void update();
	void addspawns(int a_spawns);
};