#include "Creature.h"

void Creature::init() {
	active = false;
}

bool Creature::isActive() { 
	return active; 
}


void Creature::deactivate() { 
	active = false; 
}


void Creature::spawn(char t, char d, vector l) {
	type = t;
	dir = d;
	pos = l;
	timeSinceUpdate = 0;
	speed = 0;
	active = true;

}

void Creature::setDir(char d) {
	dir = d;
}


char Creature::getDir() { 
	return dir; 
}


void Creature::setSpeed(float a_speed) {
	speed = a_speed;
}


vector Creature::nextPos(float a_speedmultiplier) {
	if (dir == 0) {
		return vector(pos.x, pos.y + (speed*a_speedmultiplier*Engine::instance()->dt()), pos.z);
	}
	else if (dir == 1) {
		return vector(pos.x + (speed*a_speedmultiplier*Engine::instance()->dt()), pos.y, pos.z);
	}
	else if (dir == 2) {
		return vector(pos.x, pos.y - (speed*a_speedmultiplier*Engine::instance()->dt()), pos.z);
	}
	else if (dir == 3) {
		return vector(pos.x - (speed*a_speedmultiplier*Engine::instance()->dt()), pos.y, pos.z);
	}



	return pos;
}


vector Creature::curPos() { 
	return pos; 
}


void Creature::update() {

}





