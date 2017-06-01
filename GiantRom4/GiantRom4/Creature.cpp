#include "Creature.h"



Creature::Creature() {

}


Creature::Creature(char a_type, char a_dir, vector a_loc) {
	if (a_type == 0) { // pup
		spri[0].image = (imageAsset*)((Engine::instance()->getResource("pupup.png", 0xFF000000))->resource);
		spri[0].rec.left = 0;
		spri[0].rec.top = 0;
		spri[0].rec.right = spri[0].image->texInfo.Width;
		spri[0].rec.bottom = spri[0].image->texInfo.Height;
		spri[0].center.x = spri[0].rec.right / 2;
		spri[0].center.y = spri[0].rec.bottom / 2;
		spri[0].center.z = 0;
		spri[0].color = 0xFFFFFFFF;
		spri[1].image = (imageAsset*)((Engine::instance()->getResource("pupright.png", 0xFF000000))->resource);
		spri[1].rec.left = 0;
		spri[1].rec.top = 0;
		spri[1].rec.right = spri[1].image->texInfo.Width;
		spri[1].rec.bottom = spri[1].image->texInfo.Height;
		spri[1].center.x = spri[1].rec.right / 2;
		spri[1].center.y = spri[1].rec.bottom / 2;
		spri[1].center.z = 0;
		spri[1].color = 0xFFFFFFFF;
		spri[2].image = (imageAsset*)((Engine::instance()->getResource("pupdown.png", 0xFF000000))->resource);
		spri[2].rec.left = 0;
		spri[2].rec.top = 0;
		spri[2].rec.right = spri[2].image->texInfo.Width;
		spri[2].rec.bottom = spri[2].image->texInfo.Height;
		spri[2].center.x = spri[2].rec.right / 2;
		spri[2].center.y = spri[2].rec.bottom / 2;
		spri[2].center.z = 0;
		spri[2].color = 0xFFFFFFFF;
		spri[3].image = (imageAsset*)((Engine::instance()->getResource("pupleft.png", 0xFF000000))->resource);
		spri[3].rec.left = 0;
		spri[3].rec.top = 0;
		spri[3].rec.right = spri[3].image->texInfo.Width;
		spri[3].rec.bottom = spri[3].image->texInfo.Height;
		spri[3].center.x = spri[3].rec.right / 2;
		spri[3].center.y = spri[3].rec.bottom / 2;
		spri[3].center.z = 0;
		spri[3].color = 0xFFFFFFFF;
	}
	else if (a_type == 1) { // rorie
		spri[0].image = (imageAsset*)((Engine::instance()->getResource("rorieup.png", 0xFF000000))->resource);
		spri[0].rec.left = 0;
		spri[0].rec.top = 0;
		spri[0].rec.right = spri[0].image->texInfo.Width;
		spri[0].rec.bottom = spri[0].image->texInfo.Height;
		spri[0].center.x = spri[0].rec.right / 2;
		spri[0].center.y = spri[0].rec.bottom / 2;
		spri[0].center.z = 0;
		spri[0].color = 0xFFFFFFFF;
		spri[1].image = (imageAsset*)((Engine::instance()->getResource("rorieright.png", 0xFF000000))->resource);
		spri[1].rec.left = 0;
		spri[1].rec.top = 0;
		spri[1].rec.right = spri[1].image->texInfo.Width;
		spri[1].rec.bottom = spri[1].image->texInfo.Height;
		spri[1].center.x = spri[1].rec.right / 2;
		spri[1].center.y = spri[1].rec.bottom / 2;
		spri[1].center.z = 0;
		spri[1].color = 0xFFFFFFFF;
		spri[2].image = (imageAsset*)((Engine::instance()->getResource("roriedown.png", 0xFF000000))->resource);
		spri[2].rec.left = 0;
		spri[2].rec.top = 0;
		spri[2].rec.right = spri[2].image->texInfo.Width;
		spri[2].rec.bottom = spri[2].image->texInfo.Height;
		spri[2].center.x = spri[2].rec.right / 2;
		spri[2].center.y = spri[2].rec.bottom / 2;
		spri[2].center.z = 0;
		spri[2].color = 0xFFFFFFFF;
		spri[3].image = (imageAsset*)((Engine::instance()->getResource("rorieleft.png", 0xFF000000))->resource);
		spri[3].rec.left = 0;
		spri[3].rec.top = 0;
		spri[3].rec.right = spri[3].image->texInfo.Width;
		spri[3].rec.bottom = spri[3].image->texInfo.Height;
		spri[3].center.x = spri[3].rec.right / 2;
		spri[3].center.y = spri[3].rec.bottom / 2;
		spri[3].center.z = 0;
		spri[3].color = 0xFFFFFFFF;
	}
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
	speed = 0;
	active = true;

}

void Creature::setDir(char a_direction) {
	dir = a_direction;
}


char Creature::getDir() { 
	return dir; 
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


vector Creature::curPos() { 
	return pos; 
}


void Creature::update(float a_speedmultiplier) {
	setPos(nextPos(a_speedmultiplier));
}





