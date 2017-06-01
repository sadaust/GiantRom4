#pragma once
#include "Engine.h"
#include "ChompDefines.h"

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
	float speed;
	spriteStruct spri[NUMCARDINALDIRECTIONS];
public:
	Creature();
	Creature(char a_type, char a_dir, vector a_loc);
	void init();
	bool isActive();
	void deactivate();
	void spawn(char a_type, char a_direction, vector a_loc);
	void setDir(char a_dir);
	char getDir();
	void setSpeed(float a_speed);
	vector nextPos(float a_speedmultiplier);
	void setPos(vector a_pos);
	vector curPos();
	void update(float a_speedmultiplier);
	//renInfo getRen();
};