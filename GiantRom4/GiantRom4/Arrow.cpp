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

bool Arrow::isActive() {
	return active;
}

char Arrow::getRot() {
	return rot;
}

vector Arrow::getPos() {
	return pos;
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

renInfo Arrow::getRen() {
	renInfo ren;

	ren.asset = &spri;
	ren.type = screenSprite;
	D3DXMatrixIdentity(&ren.matrix);
	return ren;
}