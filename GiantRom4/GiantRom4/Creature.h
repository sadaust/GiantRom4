#pragma once
#include "Engine.h"

//#define creatureTypes 3
#define PUPTYPE 0
#define RORIETYPE 1
#define SPECPUPTYPE 2

class Creature {
private:
	char type;
	char dir;
	vector pos;
	bool active;
	double timeSinceUpdate;
	float speed;
	//spriteStruct ren;
	//imageAsset* assets[creatureTypes];
public:
	void init();
	bool isActive();
	void deactivate();
	void spawn(char type, char direction, vector loc);
	void setDir(char dir);
	char getDir();
	void setSpeed(float a_speed);
	vector nextPos(float a_speedmultiplier);
	vector curPos();
	void update();
	//renInfo getRen();
};