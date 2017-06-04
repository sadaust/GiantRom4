#include "Creature.h"



Creature::Creature() {

}


Creature::Creature(char a_type, char a_dir, vector a_loc) {
	spawn(a_type, a_dir, a_loc);
}


void Creature::init() {
	active = false;
}


bool Creature::isActive() { 
	return active; 
}


void Creature::deactivate() { 
	active = false; 
}


void Creature::spawn(char a_type, char a_direction, vector a_loc) {
	type = a_type;
	dir = a_direction;
	pos = a_loc;
	if (type == 0)
		speed = PUPSPEED;
	if (type == 1)
		speed = RORIESPEED;
	active = true;

}

void Creature::setDir(char a_direction) {
	if (a_direction >= 0 && a_direction <= NUMCARDINALDIRECTIONS)
		dir = a_direction;
}


char Creature::getDir() { 
	return dir; 
}


char Creature::getType() {
	return type;
}


void Creature::setSpeed(float a_speed) {
	speed = a_speed;
}


vector Creature::nextPos(float a_speedmultiplier) {
	switch (dir) {
	case 0:
		return vector(pos.x, pos.y - (speed*a_speedmultiplier*Engine::instance()->dt()), pos.z);
	case 1:
		return vector(pos.x + (speed*a_speedmultiplier*Engine::instance()->dt()), pos.y, pos.z);
	case 2:
		return vector(pos.x, pos.y + (speed*a_speedmultiplier*Engine::instance()->dt()), pos.z);
	case 3: 
		return vector(pos.x - (speed*a_speedmultiplier*Engine::instance()->dt()), pos.y, pos.z);
	default:
		return pos;
	}
}


void Creature::setPos(vector a_pos) {
	pos = a_pos;
}


vector Creature::getPos() { 
	return pos; 
}


void Creature::update(float a_speedmultiplier) {
	setPos(nextPos(a_speedmultiplier));
}







