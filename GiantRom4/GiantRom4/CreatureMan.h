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
	std::vector <float> timetospawn; // for each spawner.
	float minTimeBetweenSpawns, maxTimeBetweenSpawns;
	float speedmultiplier;
	spriteStruct creaturesprites[creatureTypes][numCreatureSprites];

	void ClearCreatures();
	void ClearSpawners();
public:
	CreatureMan();
	void Init(float a_minTimeBetweenSpawns, float a_maxTimeBetweenSpawns, float a_speedmultiplier);
	void Update();
	void Render();
	void setSpeed(float a_speed);
	void addSpawner(Spawner a_spawn);
};