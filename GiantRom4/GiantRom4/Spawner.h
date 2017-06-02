#pragma once
#include "Engine.h"





class Spawner {
private:
	vector pos;
	char dir;
	float timetospawn;
	bool active, spawnnow;
public:
	Spawner();
	Spawner(vector a_pos, char a_dir, bool a_active);
	void Init(vector a_pos, char a_dir, bool a_active);
	void activate(vector a_pos, char a_dir);
	void Update();
	void setNewTimeToSpawn(float a_min, float a_max);
	void setPos(vector a_pos);
	void setDir(char a_dir);
	void setActive(bool a_active);
	vector getPos();
	char getDir();
	bool isActive();
	bool doISpawn();
	void resetDoISpawn();
};