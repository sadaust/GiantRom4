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
	if (dir == 0)
		pos.y += AVERYTINYNUMBER;
	else if (dir == 1)
		pos.x -= AVERYTINYNUMBER;
	else if (dir == 2)
		pos.y -= AVERYTINYNUMBER;
	else if (dir == 3)
		pos.x += AVERYTINYNUMBER;
	if (type == 0)
		speed = PUPSPEED;
	if (type == 1)
		speed = RORIESPEED;
	active = true;
	lastarrow = -1;

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
		if (pos.y == round(pos.y))
			return true;
		target = floor(pos.y);
		if (target < pos.y - a_distancetotravel)
			return false;
		else
			return true;
	case 1:
		if (pos.x == round(pos.x))
			return true;
		target = ceil(pos.x);
		if (target > pos.x + a_distancetotravel)
			return false;
		else
			return true;
	case 2:
		if (pos.y == round(pos.y))
			return true;
		target = ceil(pos.y);
		if (target > pos.y + a_distancetotravel)
			return false;
		else
			return true;
	case 3:
		if (pos.x == round(pos.x))
			return true;
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
	lastarrow = -1;
}


void Creature::update(float a_speedmultiplier, std::vector <Wall> & a_walls, std::vector <Arrow> & a_arrows) {
	bool collisiondir, arrowcollided, directionchange;
	std::vector <int> collidedArrows;
	vector tempwallpos;
	float traveldistance = distanceToTravel(a_speedmultiplier);
	char newdirection = dir, numcollisions = 0;
	bool donemoving = false, collided;
	while (!donemoving) {
		collided = false;
		arrowcollided = false;
		directionchange = false;
		switch (newdirection) {
		case 0:
			collisiondir = true;
			for (int i = 0; i < a_arrows.size() && !arrowcollided; ++i) {
				if (a_arrows[i].isActive() && i != lastarrow) {
					tempwallpos = a_arrows[i].getPos();
					if (a_arrows[i].getRot() != newdirection) {
						if (tempwallpos.x == pos.x) {
							if (tempwallpos.y < pos.y && tempwallpos.y >= pos.y - traveldistance) {
								traveldistance -= abs(pos.y - tempwallpos.y);
								pos.y = tempwallpos.y;
								newdirection = a_arrows[i].getRot();
								arrowcollided = true;
								if (newdirection == 0)				// move slightly backwards according to your new direction
									pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
								else if (newdirection == 1)
									pos.x -= AVERYTINYNUMBER;
								else if (newdirection == 2)
									pos.y -= AVERYTINYNUMBER;
								else if (newdirection == 3)
									pos.x += AVERYTINYNUMBER;
								traveldistance += AVERYTINYNUMBER;
								directionchange = true;
								lastarrow = i;
							}
						}
					}
				}
			}
			if (pos.y - traveldistance <= 0.0f && !directionchange) {
				traveldistance -= abs(pos.y);
				pos.y = 0.0f;
				if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
					newdirection += 1;
				else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
					newdirection += 2;
				else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
					newdirection -= 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection -= NUMCARDINALDIRECTIONS;
				else if (newdirection < 0)
					newdirection += NUMCARDINALDIRECTIONS;
				collided = true;
				if (newdirection == 0)				// move slightly backwards according to your new direction
					pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
				else if (newdirection == 1)
					pos.x -= AVERYTINYNUMBER;
				else if (newdirection == 2)
					pos.y -= AVERYTINYNUMBER;
				else if (newdirection == 3)
					pos.x += AVERYTINYNUMBER;
				traveldistance += AVERYTINYNUMBER;
				directionchange = true;
				++numcollisions;
				if (numcollisions > COLLISIONRESET)
					numcollisions = 0;
			}
			for (int i = 0; i < a_walls.size() && !collided && !directionchange; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.x == pos.x) {
						if (tempwallpos.y < pos.y && tempwallpos.y >= pos.y - traveldistance) {
							traveldistance -= abs(pos.y - tempwallpos.y);
							pos.y = tempwallpos.y;
							if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
								newdirection += 1;
							else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
								newdirection += 2;
							else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
								newdirection -= 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection -= NUMCARDINALDIRECTIONS;
							else if (newdirection < 0)
								newdirection += NUMCARDINALDIRECTIONS;
							collided = true;
							if (newdirection == 0)				// move slightly backwards according to your new direction
								pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
							else if (newdirection == 1)
								pos.x -= AVERYTINYNUMBER;
							else if (newdirection == 2)
								pos.y -= AVERYTINYNUMBER;
							else if (newdirection == 3)
								pos.x += AVERYTINYNUMBER;
							traveldistance += AVERYTINYNUMBER;
							directionchange = true;
							++numcollisions;
							if (numcollisions > COLLISIONRESET)
								numcollisions = 0;
						}
					}
				}
			}
			if (!collided && !directionchange) {
				pos.y -= traveldistance;
				traveldistance = 0.0f;
			}
			break;
		case 1:
			collisiondir = false;
			for (int i = 0; i < a_arrows.size() && !arrowcollided; ++i) {
				if (a_arrows[i].isActive() && i != lastarrow) {
					tempwallpos = a_arrows[i].getPos();
					if (a_arrows[i].getRot() != newdirection) {
						if (tempwallpos.y == pos.y) {
							if (tempwallpos.x > pos.x && tempwallpos.x <= pos.x + traveldistance) {
								traveldistance -= abs(tempwallpos.x - pos.x);
								pos.x = tempwallpos.x;
								newdirection = a_arrows[i].getRot();
								arrowcollided = true;
								if (newdirection == 0)				// move slightly backwards according to your new direction
									pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
								else if (newdirection == 1)
									pos.x -= AVERYTINYNUMBER;
								else if (newdirection == 2)
									pos.y -= AVERYTINYNUMBER;
								else if (newdirection == 3)
									pos.x += AVERYTINYNUMBER;
								traveldistance += AVERYTINYNUMBER;
								directionchange = true;
								lastarrow = i;
							}
						}
					}
				}
			}
			if (pos.x + traveldistance >= MAPSIZE_X - 1.0f && !directionchange) {
				traveldistance -= abs((MAPSIZE_X - 1.0f) - pos.x);
				pos.x = MAPSIZE_X - 1.0f;
				if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
					newdirection += 1;
				else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
					newdirection += 2;
				else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
					newdirection -= 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection -= NUMCARDINALDIRECTIONS;
				else if (newdirection < 0)
					newdirection += NUMCARDINALDIRECTIONS;
				collided = true;
				if (newdirection == 0)				// move slightly backwards according to your new direction
					pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
				else if (newdirection == 1)
					pos.x -= AVERYTINYNUMBER;
				else if (newdirection == 2)
					pos.y -= AVERYTINYNUMBER;
				else if (newdirection == 3)
					pos.x += AVERYTINYNUMBER;
				traveldistance += AVERYTINYNUMBER;
				directionchange = true;
				++numcollisions;
				if (numcollisions > COLLISIONRESET)
					numcollisions = 0;
			}
			for (int i = 0; i < a_walls.size() && !collided && !directionchange; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.y == pos.y) {
						if (tempwallpos.x - 1.0f > pos.x && tempwallpos.x - 1.0f <= pos.x + traveldistance) {
							traveldistance -= abs(tempwallpos.x - pos.x);
							pos.x = tempwallpos.x - 1.0f;
							if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
								newdirection += 1;
							else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
								newdirection += 2;
							else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
								newdirection -= 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection -= NUMCARDINALDIRECTIONS;
							else if (newdirection < 0)
								newdirection += NUMCARDINALDIRECTIONS;
							collided = true;
							if (newdirection == 0)				// move slightly backwards according to your new direction
								pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
							else if (newdirection == 1)
								pos.x -= AVERYTINYNUMBER;
							else if (newdirection == 2)
								pos.y -= AVERYTINYNUMBER;
							else if (newdirection == 3)
								pos.x += AVERYTINYNUMBER;
							traveldistance += AVERYTINYNUMBER;
							directionchange = true;
							++numcollisions;
							if (numcollisions > COLLISIONRESET)
								numcollisions = 0;
						}
					}
				}
			}
			if (!collided && !directionchange) {
				pos.x += traveldistance;
				traveldistance = 0.0f;
			}
			break;
		case 2:
			collisiondir = true;
			for (int i = 0; i < a_arrows.size() && !arrowcollided; ++i) {
				if (a_arrows[i].isActive() && i != lastarrow) {
					tempwallpos = a_arrows[i].getPos();
					if (a_arrows[i].getRot() != newdirection) {
						if (tempwallpos.x == pos.x) {
							if (tempwallpos.y > pos.y && tempwallpos.y <= pos.y + traveldistance) {
								traveldistance -= abs(tempwallpos.y - pos.y);
								pos.y = tempwallpos.y;
								newdirection = a_arrows[i].getRot();
								arrowcollided = true;
								if (newdirection == 0)				// move slightly backwards according to your new direction
									pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
								else if (newdirection == 1)
									pos.x -= AVERYTINYNUMBER;
								else if (newdirection == 2)
									pos.y -= AVERYTINYNUMBER;
								else if (newdirection == 3)
									pos.x += AVERYTINYNUMBER;
								traveldistance += AVERYTINYNUMBER;
								directionchange = true;
								lastarrow = i;
							}
						}
					}
				}
			}
			if (pos.y + traveldistance >= MAPSIZE_Y && !directionchange) {
				traveldistance -= abs(MAPSIZE_Y - pos.y);
				pos.y = MAPSIZE_Y;
				if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
					newdirection += 1;
				else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
					newdirection += 2;
				else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
					newdirection -= 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection -= NUMCARDINALDIRECTIONS;
				else if (newdirection < 0)
					newdirection += NUMCARDINALDIRECTIONS;
				collided = true;
				if (newdirection == 0)				// move slightly backwards according to your new direction
					pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
				else if (newdirection == 1)
					pos.x -= AVERYTINYNUMBER;
				else if (newdirection == 2)
					pos.y -= AVERYTINYNUMBER;
				else if (newdirection == 3)
					pos.x += AVERYTINYNUMBER;
				traveldistance += AVERYTINYNUMBER;
				directionchange = true;
				++numcollisions;
				if (numcollisions > COLLISIONRESET)
					numcollisions = 0;
			}
			for (int i = 0; i < a_walls.size() && !collided && !directionchange; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.x == pos.x) {
						if (tempwallpos.y - 1.0f > pos.y && tempwallpos.y - 1.0f <= pos.y + traveldistance) {
							traveldistance -= abs(tempwallpos.y - pos.y);
							pos.y = tempwallpos.y - 1.0f;
							if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
								newdirection += 1;
							else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
								newdirection += 2;
							else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
								newdirection -= 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection -= NUMCARDINALDIRECTIONS;
							else if (newdirection < 0)
								newdirection += NUMCARDINALDIRECTIONS;
							collided = true;
							if (newdirection == 0)				// move slightly backwards according to your new direction
								pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
							else if (newdirection == 1)
								pos.x -= AVERYTINYNUMBER;
							else if (newdirection == 2)
								pos.y -= AVERYTINYNUMBER;
							else if (newdirection == 3)
								pos.x += AVERYTINYNUMBER;
							traveldistance += AVERYTINYNUMBER;
							directionchange = true;
							++numcollisions;
							if (numcollisions > COLLISIONRESET)
								numcollisions = 0;
						}
					}
				}
			}
			if (!collided && !directionchange) {
				pos.y += traveldistance;
				traveldistance = 0.0f;
			}
			break;
		case 3:
			collisiondir = false;
			for (int i = 0; i < a_arrows.size() && !arrowcollided; ++i) {
				if (a_arrows[i].isActive() && i != lastarrow) {
					tempwallpos = a_arrows[i].getPos();
					if (a_arrows[i].getRot() != newdirection) {
						if (tempwallpos.y == pos.y) {
							if (tempwallpos.x < pos.x && tempwallpos.x >= pos.x - traveldistance) {
								traveldistance -= abs(pos.x - tempwallpos.x);
								pos.x = tempwallpos.x;
								newdirection = a_arrows[i].getRot();
								arrowcollided = true;
								if (newdirection == 0)				// move slightly backwards according to your new direction
									pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
								else if (newdirection == 1)
									pos.x -= AVERYTINYNUMBER;
								else if (newdirection == 2)
									pos.y -= AVERYTINYNUMBER;
								else if (newdirection == 3)
									pos.x += AVERYTINYNUMBER;
								traveldistance += AVERYTINYNUMBER;
								directionchange = true;
								lastarrow = i;
							}
						}
					}
				}
			}
			if (pos.x - traveldistance <= 0.0f && !directionchange) {
				traveldistance -= abs(pos.x);
				pos.x = 0.0f;
				if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
					newdirection += 1;
				else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
					newdirection += 2;
				else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
					newdirection -= 1;
				if (newdirection >= NUMCARDINALDIRECTIONS)
					newdirection -= NUMCARDINALDIRECTIONS;
				else if (newdirection < 0)
					newdirection += NUMCARDINALDIRECTIONS;
				collided = true;
				if (newdirection == 0)				// move slightly backwards according to your new direction
					pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
				else if (newdirection == 1)
					pos.x -= AVERYTINYNUMBER;
				else if (newdirection == 2)
					pos.y -= AVERYTINYNUMBER;
				else if (newdirection == 3)
					pos.x += AVERYTINYNUMBER;
				traveldistance += AVERYTINYNUMBER;
				directionchange = true;
				++numcollisions;
				if (numcollisions > COLLISIONRESET)
					numcollisions = 0;
			}
			for (int i = 0; i < a_walls.size() && !collided && !directionchange; ++i) {
				if (collisiondir == a_walls[i].amIHorizontal()) {
					tempwallpos = a_walls[i].getPos();
					if (tempwallpos.y == pos.y) {
						if (tempwallpos.x < pos.x && tempwallpos.x >= pos.x - traveldistance) {
							traveldistance -= abs(pos.x - tempwallpos.x);
							pos.x = tempwallpos.x;
							if (numcollisions == 0) // if you're colliding for the first time, turn right. (right from the forward you are currently facing)
								newdirection += 1;
							else if (numcollisions == 1)// if you're colliding for the second time turn left. (backwards from the right you are currently facing)
								newdirection += 2;
							else if (numcollisions == 2)// if you're colliding for the third time turn backwards. (left from the left you are currently facing)
								newdirection -= 1;
							if (newdirection >= NUMCARDINALDIRECTIONS)
								newdirection -= NUMCARDINALDIRECTIONS;
							else if (newdirection < 0)
								newdirection += NUMCARDINALDIRECTIONS;
							collided = true;
							if (newdirection == 0)				// move slightly backwards according to your new direction
								pos.y += AVERYTINYNUMBER;		// to facilitate colliding with walls correctly
							else if (newdirection == 1)
								pos.x -= AVERYTINYNUMBER;
							else if (newdirection == 2)
								pos.y -= AVERYTINYNUMBER;
							else if (newdirection == 3)
								pos.x += AVERYTINYNUMBER;
							traveldistance += AVERYTINYNUMBER;
							directionchange = true;
							++numcollisions;
							if (numcollisions > COLLISIONRESET)
								numcollisions = 0;
						}
					}
				}
			}
			if (!collided && !directionchange) {
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









