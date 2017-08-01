#include "Player.h"

void Player::init() {
	cursor.image = (imageAsset*) ((Engine::instance()->getResource("selector.png",0xFF000000))->resource);
	cursor.rec.top = 0;
	cursor.rec.left = 0;
	cursor.rec.bottom = cursor.image->texInfo.Height;
	cursor.rec.right = cursor.image->texInfo.Width;
	cursor.center.x = cursor.rec.right/2.0f;
	cursor.center.y = cursor.rec.bottom/2.0f;
	cursor.center.z = 0;

	base.image = (imageAsset*) ((Engine::instance()->getResource("base.png",0xFF000000))->resource);
	base.rec.top = 0;
	base.rec.left = 0;
	base.rec.bottom = base.image->texInfo.Height;
	base.rec.right = base.image->texInfo.Width;
	base.center.x = base.rec.right/2.0f;
	base.center.y = base.rec.bottom/2.0f;
	base.center.z = 0;

	color = 0xFFFFFFFF;
	cursor.color = color;
	base.color = color;
	cPos = vector(0,0,0);
	goalPos = vector(0,0,0);
}

renInfo Player::getRen() {
	renInfo ren;
	if(cursor.image) {
		ren.type = screenSprite;
		ren.asset = &cursor;
		D3DXMatrixIdentity(&ren.matrix);
	}
	return ren;
}

renInfo Player::baseRen() {
	renInfo ren;
	if(base.image) {
		ren.type = screenSprite;
		ren.asset = &base;
		D3DXMatrixIdentity(&ren.matrix);
	}
	return ren;
}


D3DXCOLOR Player::getColor() {
	return color;
}

