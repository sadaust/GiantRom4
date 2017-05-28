//Match is the code for vs
#pragma once

#include "Player.h"
#include "Arrow.h"
#include "Creature.h"
#include "Spawner.h"
#include <string>
#define sizeX 12
#define sizeY 9
#define maxCreatures 1
#define MAXIMUMPLAYERS 4
#define MAXIMUMARROWS 5
#define MAXIMUMSPAWNERS 8
#define SMALLARROWTIME 2
#define SMALLARROWMULTIPLIER 0.6f

class Match {
private:
	float gridWidth;
	float gridHeight;
	int numplayers;
	int numarrows;
	int numspawners;
	double arrowdecaytime;
	Player players[MAXIMUMPLAYERS];
	Arrow arrows[MAXIMUMPLAYERS][MAXIMUMARROWS];
	int curArrow[MAXIMUMPLAYERS];
	Creature critters[maxCreatures];
	std::string inputInfo[MAXIMUMPLAYERS][8];
	Spawner spawners[MAXIMUMSPAWNERS];

	void snapToNextAvailableArrow(int a_player);
public:
	void init();
	void render();
	void update();
};