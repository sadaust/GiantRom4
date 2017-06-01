#include "CreatureMan.h"




CreatureMan::CreatureMan() {
	minTimeBetweenSpawns = 0.0f;
	maxTimeBetweenSpawns = 0.01f;
	speedmultiplier = 1.0f;
}


void CreatureMan::Init(float a_minTimeBetweenSpawns, float a_maxTimeBetweenSpawns, float a_speedmultiplier) {
	minTimeBetweenSpawns = a_minTimeBetweenSpawns;
	maxTimeBetweenSpawns = a_maxTimeBetweenSpawns;
	speedmultiplier = a_speedmultiplier;
}


void CreatureMan::Update() {

}


void CreatureMan::Render() {

}


void CreatureMan::setSpeed(float a_speed) {
	speedmultiplier = a_speed;
}


void CreatureMan::addSpawner(Spawner a_spawn) {
	spawners.push_back(a_spawn);
}


void CreatureMan::ClearCreatures() {
	creatures.clear();
}


void CreatureMan::ClearSpawners() {
	spawners.clear();
}



