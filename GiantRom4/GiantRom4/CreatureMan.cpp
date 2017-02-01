#include "CreatureMan.h"



CreatureMan::CreatureMan() {
	numtospawn = 0;
	timebetweenspawns = 0.001f;
	speedmultiplier = 1.0f;
}


void CreatureMan::init() {

}


void CreatureMan::update() {

}


void CreatureMan::addspawns(int a_spawns) {
	numtospawn += a_spawns;
	if (numtospawn > MAXTOSPAWN)
		numtospawn = MAXTOSPAWN;
}





