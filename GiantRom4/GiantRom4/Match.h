//Match is the code for vs
#pragma once

#include "Player.h"
#include "Arrow.h"
#include <string>
#define sizeX 12
#define sizeY 9
class Match {
private:
	float gridWidth;
	float gridHeight;
	Player players[2];
	Arrow arrows[2][3];
	int curArrow[2];
	std::string inputInfo[2][8];
public:
	void init();
	void render();
	void update();
};