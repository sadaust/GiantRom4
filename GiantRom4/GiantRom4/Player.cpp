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
	color = 0xFFFFFFFF;
	cursor.color = color;
	cPos = vector(0,0,0);
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