#include "Spawner.h"




Spawner::Spawner() {
	color = 0xFFFFFFFF;
	active = false;
	spri.image = 0;
}


void Spawner::init() {
	active = false;

	spri.image = (imageAsset*)((Engine::instance()->getResource("spawner.png", 0xFF000000))->resource);

	spri.rec.left = 0;
	spri.rec.top = 0;
	spri.rec.right = spri.image->texInfo.Width;
	spri.rec.bottom = spri.image->texInfo.Height;

	spri.center.x = spri.rec.right / 2;
	spri.center.y = spri.rec.bottom / 2;
	spri.center.z = 0;

	spri.color = color;
}


void Spawner::activate(vector a_pos){
	setPos(a_pos);
	setActive(true);
}


void Spawner::update() {
	// put code here for different spawning modes.
	// Random will have a percent chance of spawning each tick.
	// Constant will have a set interval before a new spawn.
	// Dog mode, rorie mode, cluster mode, insanity mode, direction mode, etc. Mode is held outside of spawner and can probably be passed in.
}


void Spawner::setPos(vector a_pos) {
	pos = a_pos;
}


void Spawner::setColor(D3DXCOLOR a_col) {
	color = a_col;
	spri.color = color;
}


void Spawner::setActive(bool a_active) {
	active = a_active;
}


vector Spawner::getPos() {
	return pos;
}


renInfo Spawner::getRen() {
	renInfo ren;

	ren.asset = &spri;
	ren.type = screenSprite;
	D3DXMatrixIdentity(&ren.matrix);
	return ren;
}


bool Spawner::isActive() {
	return active;
}