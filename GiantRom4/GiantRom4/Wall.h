#pragma once
#include "Engine.h"
#include "ChompDefines.h"













class Wall {
private:
	vector pos;
	bool horizontal; // true = horizontal AKA right, false = veritcal AKA down

public:
	Wall();
};