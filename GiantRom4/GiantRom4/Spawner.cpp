#include "Spawner.h"




Spawner::Spawner() {
	active = false;
	dir = 0;
}


Spawner::Spawner(vector a_pos, char a_dir, bool a_active) {
	Init(a_pos, a_dir, a_active);
}


void Spawner::Init(vector a_pos, char a_dir, bool a_active) {
	active = a_active;
	pos = a_pos;
	dir = a_dir;
	spawnnow = false;
}


void Spawner::activate(vector a_pos, char a_dir){
	setPos(a_pos);
	setActive(true);
	setDir(a_dir);
}


void Spawner::Update() {
	timetospawn -= Engine::instance()->dt();
	if (timetospawn <= 0) {
		spawnnow = true;
	}
	// put code here for different spawning modes.
	// Random will have a percent chance of spawning each tick.
	// Constant will have a set interval before a new spawn.
	// Dog mode, rorie mode, cluster mode, insanity mode, direction mode, etc. Mode is held outside of spawner and can probably be passed in.
}


void Spawner::setNewTimeToSpawn(float a_min, float a_max) {
	timetospawn = a_min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (a_max - a_min)));
}


void Spawner::setPos(vector a_pos) {
	pos = a_pos;
}


void Spawner::setDir(char a_dir) {
	dir = a_dir;
}


void Spawner::setActive(bool a_active) {
	active = a_active;
}


vector Spawner::getPos() {
	return pos;
}


char Spawner::getDir() {
	return dir;
}


bool Spawner::isActive() {
	return active;
}


bool Spawner::doISpawn() {
	return spawnnow;
}


void Spawner::resetDoISpawn() {
	spawnnow = false;
}








