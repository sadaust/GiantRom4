#pragma once
#include "Engine.h"
#include "ChompDefines.h"













class Wall {
private:
	vector pos;
	bool horizontal; // true = horizontal AKA right, false = vertical AKA down

public:
	Wall();
	Wall(vector a_pos, bool a_horizontal);
	vector getPos();
	bool amIHorizontal();
	void setPos(vector a_pos);
	void setHorizontal(bool a_horizontal);
};