#pragma once
#include "Engine.h"





class Spawner {
private:
	vector pos;
	char rot;
	spriteStruct spri;
	D3DXCOLOR color;
public:
	Spawner();
	void init(vector a_pos, char a_rot);
	void update();
	void setPos(vector a_pos);
	void setRot(char a_rot);
	vector getPos();
	char getRot();
	renInfo getRen();
};