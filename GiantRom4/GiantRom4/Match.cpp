#include "Match.h"

void Match::init() {
	gridWidth = Engine::instance()->getWidth()*0.85f;
	gridHeight = Engine::instance()->getHeight()*0.85f;
	gridWidth /= MAPSIZE_X;
	gridHeight /= MAPSIZE_Y;
	GameplayMan.Init(gridWidth, gridHeight, DEFAULTMINSPAWNTIME, DEFAULTMAXSPAWNTIME, SMALLARROWTIME * 2.0f,DEFAULTMINSPAWNTIME,DEFAULTMAXSPAWNTIME,1.0f,15);

	//for (int i = 0; i < numspawners; ++i) {
	//	spawners[i].init();
	//	spawners[i].setPos(vector(2+(3*i),6,0));
	//	spawners[i].activate(vector(2 + (3 * i), 6, 0),0);
	//}

}

void Match::render() {
	GameplayMan.Render();
}

void Match::update() {
	//vector pos;
	//bool empty = true;
	//for (int i = 0; i < numplayers; ++i) {
	//	//for (int g = 0; g < numarrows; ++g) {
	//	//	arrows[i][g].update();
	//	//}
	//	pos = players[i].getCur();
	//	if (Engine::instance()->getFlags(inputInfo[i][0].c_str())&buttonFlags::_repeat) {
	//		pos.y -= 1;
	//	}
	//	else if (Engine::instance()->getFlags(inputInfo[i][1].c_str())&buttonFlags::_repeat) {
	//		pos.x += 1;
	//	}
	//	if (Engine::instance()->getFlags(inputInfo[i][2].c_str())&buttonFlags::_repeat) {
	//		pos.y += 1;
	//	}
	//	else if (Engine::instance()->getFlags(inputInfo[i][3].c_str())&buttonFlags::_repeat) {
	//		pos.x -= 1;
	//	}

	//	if (pos.y >= MAPSIZE_Y) {
	//		pos.y = MAPSIZE_Y - 1;
	//	}
	//	else if (pos.y < 0) {
	//		pos.y = 0;
	//	}

	//	if (pos.x >= MAPSIZE_X) {
	//		pos.x = MAPSIZE_X - 1;
	//	}
	//	else if (pos.x < 0) {
	//		pos.x = 0;
	//	}
	//	players[i].setCur(pos);
	//	empty = true;
	//	for (int g = MAXIMUMPLAYERS; g < MAXIMUMPLAYERS + MAXIMUMPLAYERS; ++g) {
	//		if (Engine::instance()->getFlags(inputInfo[i][g].c_str())&buttonFlags::_repeat) {
	//			arrowman.AddArrow(i, g, players[i].getCur());
	//		}
	//	}
	//}
	// debug creature spawning
	//for (int i = 0; i < 4; ++i) {
	//	if (Engine::instance()->getFlags(debugInputInfo[i].c_str())&buttonFlags::_repeat) {
	//		critters.push_back(Creature());
	//	}
	//}
	GameplayMan.Update();
}
