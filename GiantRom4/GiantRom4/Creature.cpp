#include "Creature.h"

void Creature::init() {

}

void Creature::setDir(char d) {
	dir = d;
}

void Creature::spawn(char t, char d, vector l) {
	type = t;
	dir = d;
	pos = l;
	timeSinceUpdate = 0;
	speed = 0;
	active = true;

}