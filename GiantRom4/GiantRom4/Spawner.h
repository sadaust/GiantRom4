#pragma once
#include "Engine.h"





class Spawner {
private:
	vector pos;
	spriteStruct spri;
	char dir;
	D3DXCOLOR color;
	bool active;
public:
	Spawner();
	void init();
	void activate(vector a_pos, char a_dir);
	void update();
	void setPos(vector a_pos);
	void setDir(char a_dir);
	void setColor(D3DXCOLOR a_col);
	void setActive(bool a_active);
	vector getPos();
	renInfo getRen();
	char getDir();
	bool isActive();
};