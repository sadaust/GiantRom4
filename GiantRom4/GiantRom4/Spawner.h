#pragma once
#include "Engine.h"





class Spawner {
private:
	vector pos;
	spriteStruct spri;
	D3DXCOLOR color;
	bool active;
public:
	Spawner();
	void init();
	void activate(vector a_pos);
	void update();
	void setPos(vector a_pos);
	void setColor(D3DXCOLOR a_col);
	void setActive(bool a_active);
	vector getPos();
	renInfo getRen();
	bool isActive();
};