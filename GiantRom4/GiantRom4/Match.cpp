#include "Match.h"

void Match::init() {
	//p1Binds
	inputInfo[0][0] = "c-Y1";
	inputInfo[0][1] = "c+Y1";
	inputInfo[0][2] = "c-X1";
	inputInfo[0][3] = "c+X1";
	inputInfo[0][4] = "up1";
	inputInfo[0][5] = "right1";
	inputInfo[0][6] = "down1";
	inputInfo[0][7] = "left1";
	//p2Binds
	inputInfo[1][0] = "c-Y2";
	inputInfo[1][1] = "c+Y2";
	inputInfo[1][2] = "c-X2";
	inputInfo[1][3] = "c+X2";
	inputInfo[1][4] = "up2";
	inputInfo[1][5] = "right2";
	inputInfo[1][6] = "down2";
	inputInfo[1][7] = "left2";



	Engine::instance()->bind(W,inputInfo[0][0].c_str());
	Engine::instance()->bind(S,inputInfo[0][1].c_str());
	Engine::instance()->bind(A,inputInfo[0][2].c_str());
	Engine::instance()->bind(D,inputInfo[0][3].c_str());
	Engine::instance()->bind(T,inputInfo[0][4].c_str());
	Engine::instance()->bind(H,inputInfo[0][5].c_str());
	Engine::instance()->bind(G,inputInfo[0][6].c_str());
	Engine::instance()->bind(F,inputInfo[0][7].c_str());

	Engine::instance()->bind(UP,inputInfo[1][0].c_str());
	Engine::instance()->bind(DOWN,inputInfo[1][1].c_str());
	Engine::instance()->bind(LEFT,inputInfo[1][2].c_str());
	Engine::instance()->bind(RIGHT,inputInfo[1][3].c_str());
	Engine::instance()->bind(NP_5,inputInfo[1][4].c_str());
	Engine::instance()->bind(NP_3,inputInfo[1][5].c_str());
	Engine::instance()->bind(NP_2,inputInfo[1][6].c_str());
	Engine::instance()->bind(NP_1,inputInfo[1][7].c_str());

	D3DXCOLOR playerCol[2];

	playerCol[0] = 0xFFFF0000;
	playerCol[1] = 0xFF0000FF;

	for(int i = 0; i < 2; ++i) {
		players[i].init();
		players[i].setColor(playerCol[i]);
		players[i].setCur(vector(3+(5*i),4,0));
		players[i].setGoal(3+(5*i),4);
		curArrow[i] = 0;
		for(int z = 0; z < 3; ++z) {
			arrows[i][z].init();
			arrows[i][z].setColor(playerCol[i]);
		}
	}

	gridWidth = Engine::instance()->getWidth()*0.75f;
	gridHeight = Engine::instance()->getHeight()*0.75f;
	gridWidth /= sizeX;
	gridHeight /= sizeY;

}

void Match::render() {
	renInfo ren;
	vector vec;
	D3DXMATRIX mat;
	D3DXMATRIX rot;
	for(int i = 0; i < 2; ++i) {
		D3DXMatrixIdentity(&mat);
		ren = players[i].getRen();
		vec = players[i].getCur();
		D3DXMatrixScaling(&ren.matrix,gridWidth/256.0f,gridHeight/256.0f,1);
		D3DXMatrixTranslation(&mat,vec.x*gridWidth+(gridWidth*0.5f),vec.y*gridHeight+(gridHeight*0.5f),vec.z);
		D3DXMatrixMultiply(&ren.matrix,&ren.matrix,&mat);
		Engine::instance()->addRender(ren);
		D3DXMatrixIdentity(&mat);
		ren = players[i].baseRen();
		vec = players[i].getGoal();
		D3DXMatrixScaling(&ren.matrix,gridWidth/256.0f,gridHeight/256.0f,1);
		D3DXMatrixTranslation(&mat,vec.x*gridWidth+(gridWidth*0.5f),vec.y*gridHeight+(gridHeight*0.5f),vec.z);
		D3DXMatrixMultiply(&ren.matrix,&ren.matrix,&mat);
		Engine::instance()->addRender(ren);

		for(int z = 0; z < 3; ++z) {
			if(arrows[i][z].isActive()) {
				D3DXMatrixIdentity(&mat);
				D3DXMatrixIdentity(&rot);
				ren = arrows[i][z].getRen();
				vec = arrows[i][z].getPos();
				D3DXMatrixScaling(&ren.matrix,gridWidth/256.0f,gridHeight/256.0f,1);
				D3DXMatrixTranslation(&mat,vec.x*gridWidth+(gridWidth*0.5f),vec.y*gridHeight+(gridHeight*0.5f),0);
				D3DXMatrixRotationZ(&rot,D3DXToRadian(90.0f*arrows[i][z].getRot()));
				D3DXMatrixMultiply(&ren.matrix,&ren.matrix,&rot);
				D3DXMatrixMultiply(&ren.matrix,&ren.matrix,&mat);
				Engine::instance()->addRender(ren);
			}
		}
	}
}

void Match::update() {
	vector pos;
	bool empty = true;
	for(int i = 0; i < 2; ++i) {
		pos = players[i].getCur();
		if(Engine::instance()->getFlags(inputInfo[i][0].c_str())&buttonFlags::_repeat) {
			pos.y -= 1;
		} else if(Engine::instance()->getFlags(inputInfo[i][1].c_str())&buttonFlags::_repeat) {
			pos.y += 1;
		}
		if(Engine::instance()->getFlags(inputInfo[i][2].c_str())&buttonFlags::_repeat) {
			pos.x -= 1;
		} else if(Engine::instance()->getFlags(inputInfo[i][3].c_str())&buttonFlags::_repeat) {
			pos.x += 1;
		}

		if(pos.y >= sizeY) {
			pos.y = sizeY - 1;
		} else if(pos.y < 0) {
			pos.y = 0;
		}

		if(pos.x >= sizeX) {
			pos.x = sizeX - 1;
		} else if(pos.x < 0) {
			pos.x = 0;
		}
		players[i].setCur(pos);
		empty = true;
		for(int p = 0; p < 2; ++p) {
			if(players[i].getGoal().x == pos.x && players[i].getGoal().y == pos.y) {
				empty = false;
			}
			for(int a = 0; a < 3; ++a) {
				if(arrows[p][a].isActive()) {
					if(arrows[p][a].getPos().x == pos.x && arrows[p][a].getPos().y == pos.y) {
						empty = false;
					}
				}
			}
		}
		if(empty) {
			if(Engine::instance()->getFlags(inputInfo[i][4].c_str())&buttonFlags::_repeat) {
				arrows[i][curArrow[i]].setLoc(0,pos);
				curArrow[i] += 1;
			} else if(Engine::instance()->getFlags(inputInfo[i][5].c_str())&buttonFlags::_repeat) {
				arrows[i][curArrow[i]].setLoc(1,pos);
				curArrow[i] += 1;
			} else if(Engine::instance()->getFlags(inputInfo[i][6].c_str())&buttonFlags::_repeat) {
				arrows[i][curArrow[i]].setLoc(2,pos);
				curArrow[i] += 1;
			} else if(Engine::instance()->getFlags(inputInfo[i][7].c_str())&buttonFlags::_repeat) {
				arrows[i][curArrow[i]].setLoc(3,pos);
				curArrow[i] += 1;
			}
			if(curArrow[i] >= 3) {
				curArrow[i] = 0;
			}
		}
	}
}