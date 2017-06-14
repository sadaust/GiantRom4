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


float Creature::distanceToTravel(float a_speedmultiplier) {
	return speed*a_speedmultiplier*Engine::instance()->dt();
}


bool Creature::doICrossTheLine(float a_distancetotravel) {
	float target;
	if (a_distancetotravel >= 1.0f) {
		return true;
	}
	switch (dir) {
	case 0:
		target = floor(pos.y);
		if (target < pos.y - a_distancetotravel)
			return false;
		else
			return true;
	case 1:
		target = ceil(pos.x);
		if (target > pos.x + a_distancetotravel)
			return false;
		else
			return true;
	case 2:
		target = ceil(pos.y);
		if (target > pos.y + a_distancetotravel)
			return false;
		else
			return true;
	case 3:
		target = floor(pos.x);
		if (target < pos.x - a_distancetotravel)
			return false;
		else
			return true;
	default:
		return false;
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


void Creature::update(float a_speedmultiplier, std::vector <Wall> & a_walls) {
	bool collisiondir;
	vector tempwallpos;
	float traveldistance = distanceToTravel(a_speedmultiplier);
	char newdirection = dir;
	bool donemoving = false, collided;
	while (!donemoving) {
		collided = false;
		switch (newdirection) {
		case 0:
			collisiondir = true;
			if (pos.y - traveldistance <= 0.0f) {
				traveldistance -= abs(pos.y);
				pos.y = 0.0f;
				newdirection += 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection = 0;
				collided = true;
			}
			for (int i = 0; i < a_walls.size() && !collided; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.x == pos.x) {
						if (tempwallpos.y < pos.y && tempwallpos.y >= pos.y - traveldistance) {
							traveldistance -= abs(pos.y - tempwallpos.y);
							pos.y = tempwallpos.y;
							newdirection += 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection = 0;
							collided = true;
						}
					}
				}
			}
			if (!collided) {
				pos.y -= traveldistance;
				traveldistance = 0.0f;
			}
			break;
		case 1:
			collisiondir = false;
			if (pos.x + traveldistance >= MAPSIZE_X) {
				traveldistance -= abs(MAPSIZE_X - pos.x);
				pos.x = MAPSIZE_X;
				newdirection += 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection = 0;
				collided = true;
			}
			for (int i = 0; i < a_walls.size() && !collided; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.y == pos.y) {
						if (tempwallpos.x > pos.x && tempwallpos.x <= pos.x + traveldistance) {
							traveldistance -= abs(tempwallpos.x - pos.x);
							pos.x = tempwallpos.x;
							newdirection += 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection = 0;
							collided = true;
						}
					}
				}
			}
			if (!collided) {
				pos.x += traveldistance;
				traveldistance = 0.0f;
			}
			break;
		case 2:
			collisiondir = true;
			if (pos.y + traveldistance >= MAPSIZE_Y) {
				traveldistance -= abs(MAPSIZE_Y - pos.y);
				pos.y = MAPSIZE_Y;
				newdirection += 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection = 0;
				collided = true;
			}
			for (int i = 0; i < a_walls.size() && !collided; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.x == pos.x) {
						if (tempwallpos.y > pos.y && tempwallpos.y <= pos.y + traveldistance) {
							traveldistance -= abs(tempwallpos.y - pos.y);
							pos.y = tempwallpos.y;
							newdirection += 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection = 0;
							collided = true;
						}
					}
				}
			}
			if (!collided) {
				pos.y += traveldistance;
				traveldistance = 0.0f;
			}
			break;
		case 3:
			collisiondir = false;
			if (pos.x - traveldistance <= 0.0f) {
				traveldistance -= abs(pos.x);
				pos.x = 0.0f;
				newdirection += 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection = 0;
				collided = true;
			}
			for (int i = 0; i < a_walls.size() && !collided; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.y == pos.y) {
						if (tempwallpos.x < pos.x && tempwallpos.x <= pos.x - traveldistance) {
							traveldistance -= abs(pos.x - tempwallpos.x);
							pos.x = tempwallpos.x;
							newdirection += 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection = 0;
							collided = true;
						}
					}
				}
			}
			if (!collided) {
				pos.x -= traveldistance;
				traveldistance = 0.0f;
			}
			break;
		}




		if (traveldistance <= 0.0f)
			donemoving = true;
	}
	dir = newdirection;
}









