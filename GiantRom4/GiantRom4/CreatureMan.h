#pragma once
#include "Creature.h"
#include "Spawner.h"



#define NUMCREATURETYPES 2
#define NUMCREATURESPRITES 4


class CreatureMan {
private:
	float gridWidth;
	float gridHeight;
	std::vector <Creature> creatures;
	std::vector <Spawner> spawners;
	std::vector <float> timetospawn; // for each spawner.
	float minTimeBetweenSpawns, maxTimeBetweenSpawns;
	float speedmultiplier;
	spriteStruct creaturesprites[NUMCREATURETYPES][NUMCREATURESPRITES];
	spriteStruct spawnersprite;

	void ClearCreatures();
	void ClearSpawners();
public:
	CreatureMan();
	void Init(float a_gridwidth, float a_gridheight, float a_minTimeBetweenSpawns, float a_maxTimeBetweenSpawns, float a_speedmultiplier);
	void Update();
	void Render();
	void setSpeed(float a_speed);
	void addSpawner(Spawner a_spawn);
};