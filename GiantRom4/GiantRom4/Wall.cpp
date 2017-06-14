#pragma once
#include "Wall.h"







Wall::Wall() {
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	horizontal = true;
}


Wall::Wall(vector a_pos, bool a_horizontal) {
	setPos(a_pos);
	setHorizontal(a_horizontal);
}


vector Wall::getPos() {
	return pos;
}


bool Wall::amIHorizontal() {
	return horizontal;
}


void Wall::setPos(vector a_pos) {
	pos = a_pos;
}


void Wall::setHorizontal(bool a_horizontal) {
	horizontal = a_horizontal;
}










