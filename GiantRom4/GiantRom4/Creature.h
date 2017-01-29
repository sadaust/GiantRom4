#pragma once
#include "Engine.h"

#define creatureTypes 3

class Creature {
private:
	char type;
	char dir;
	vector pos;
	bool active;
	double timeSinceUpdate;
	float speed;
	spriteStruct ren;
	imageAsset* assets[creatureTypes];
public:
	bool isActive() {return active;}
	void spawn(char type, char direction, vector loc);
	void setDir(char dir);
	vector nextPos();
	void update();
	void render();
};