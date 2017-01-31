#include "Arrow.h"

Arrow::Arrow() {
	color = 0xFFFFFFFF;
	active = false;
	spri.image = 0;
}

void Arrow::init() {
	active = false;
	
	spri.image = (imageAsset*) ((Engine::instance()->getResource("arrow.png",0xFF000000))->resource);

	spri.rec.left = 0;
	spri.rec.top = 0;
	spri.rec.right = spri.image->texInfo.Width;
	spri.rec.bottom = spri.image->texInfo.Height;

	spri.center.x = spri.rec.right/2;
	spri.center.y = spri.rec.bottom/2;
	spri.center.z = 0;

	spri.color = color;
}

void Arrow::activate(double decaytime, char a_rot, vector a_pos) {
	setDecayTime(decaytime);
	setLoc(a_rot,a_pos);
	lasttimeupdated = Engine::instance()->dt();
	setActive(true);
}


bool Arrow::isActive() {
	return active;
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


void Arrow::setLoc(char r, vector p) {
	pos = p;
	rot = r;
	active = true;
}

void Arrow::setColor(D3DXCOLOR col) {
	color = col;
	spri.color = color;
}

void Arrow::setDecayTime(double a_decaytime) {
	decaytimeleft = a_decaytime;
}


renInfo Arrow::getRen() {
	renInfo ren;

	ren.asset = &spri;
	ren.type = screenSprite;
	D3DXMatrixIdentity(&ren.matrix);
	return ren;
}