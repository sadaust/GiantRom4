#pragma once
#include "Engine.h"

class Arrow {
private:
	vector pos;
	char rot;
	spriteStruct spri;
	D3DXCOLOR color;
	bool active;
public:
	Arrow();
	void init();
	bool isActive();
	char getRot();
	vector getPos();
	void setLoc(char rot,vector pos);
	void setActive(bool aActive) {active = aActive;}
	void setColor(D3DXCOLOR col);
	renInfo getRen();
};