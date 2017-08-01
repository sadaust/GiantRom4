#include "Arrow.h"

Arrow::Arrow() {
	color = 0xFFFFFFFF;
	active = false;

}


Arrow::Arrow(double a_decaytime, char a_rot, vector a_pos, int a_player, D3DXCOLOR a_color) {
	active = false;
	init(a_decaytime, a_rot, a_pos, a_player, a_color);
}


void Arrow::init() {
	active = false;
	player = 0;
}


void Arrow::init(double a_decaytime, char a_rot, vector a_pos, int a_player, D3DXCOLOR a_color) {
	setDecayTime(a_decaytime);
	setLoc(a_rot, a_pos);
	setPlayer(a_player);
	setColor(a_color);
}


void Arrow::activate(double decaytime, char a_rot, vector a_pos, int a_player) {
	setDecayTime(decaytime);
	setLoc(a_rot,a_pos);
	setActive(true);
	setPlayer(a_player);
}


bool Arrow::isActive() {
	return active;
}


int Arrow::getPlayer() {
	return player;
}


char Arrow::getRot() {
	return rot;
}


vector Arrow::getPos() {
	return pos;
}


double Arrow::getDecayTimeLeft() {
	return decaytimeleft;
}


void Arrow::update() {
	if (isActive()) {
		decaytimeleft -= Engine::instance()->dt();
		//lasttimeupdated = Engine::instance()->dt()/1000;
		if (decaytimeleft < 0)
			setActive(false);
	}
}


void Arrow::setPlayer(int a_player) {
	player = a_player;
}


void Arrow::setLoc(char a_rot, vector a_pos) {
	pos = a_pos;
	rot = a_rot;
}

void Arrow::setColor(D3DXCOLOR a_col) {
	color = a_col;
}

void Arrow::setDecayTime(double a_decaytime) {
	decaytimeleft = a_decaytime;
}