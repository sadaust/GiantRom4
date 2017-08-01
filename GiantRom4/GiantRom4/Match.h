//Match is the code for vs
#pragma once
#include "ChompDefines.h"
#include "Player.h"
#include "GameplayMan.h"
#include "Spawner.h"
#include <string>

class Match {
private:
	float gridWidth;
	float gridHeight;
	GameplayMan GameplayMan;
public:
	void init();
	void render();
	void update();
};