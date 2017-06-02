//Match is the code for vs
#pragma once
#include "ChompDefines.h"
#include "Player.h"
#include "ArrowMan.h"
#include "CreatureMan.h"
#include "Spawner.h"
#include <string>

class Match {
private:
	float gridWidth;
	float gridHeight;
	int numplayers;
	int numarrows;
	int numspawners;
	Player players[MAXIMUMPLAYERS];
	ArrowMan arrowman;
	CreatureMan creatureman;
	std::string inputInfo[MAXIMUMPLAYERS][8];
	std::string debugInputInfo[NUMCARDINALDIRECTIONS];
public:
	void init();
	void render();
	void update();
};