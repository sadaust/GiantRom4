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
	void init();
	bool isActive() {return active;}
	void deactivate() {active = false;}
	void spawn(char type, char direction, vector loc);
	void setDir(char dir);
	char getDir() {return dir;}
	vector nextPos();
	vector curPos() {return pos;}
	void update();
	renInfo getRen();
};