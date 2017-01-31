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

	D3DXCOLOR playerCol[MAXIMUMPLAYERS];

	playerCol[0] = 0xFFFF0000;
	playerCol[1] = 0xFF0000FF;
	playerCol[2] = 0xFF00FF00;
	playerCol[3] = 0xFFF7F83C;

	// make settings for these later
	numplayers = 2;
	numarrows = 3;
	arrowdecaytime = 6.0f;

	for(int i = 0; i < numplayers; ++i) {
		players[i].init();
		players[i].setColor(playerCol[i]);
		players[i].setCur(vector(3+(5*i),4,0));
		players[i].setGoal(3+(5*i),4);
		curArrow[i] = 0;
		for(int z = 0; z < numarrows; ++z) {
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
	for(int i = 0; i < numplayers; ++i) {
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

		for(int z = 0; z < numarrows; ++z) {
			if(arrows[i][z].isActive()) {
				D3DXMatrixIdentity(&mat);
				D3DXMatrixIdentity(&rot);
				ren = arrows[i][z].getRen();
				vec = arrows[i][z].getPos();
				if (arrows[i][z].getDecayTimeLeft() < SMALLARROWTIME)
					D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f) * SMALLARROWMULTIPLIER, (gridHeight / 256.0f) * SMALLARROWMULTIPLIER, 1);
				else
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
	for(int i = 0; i < numplayers; ++i) {
		for (int g = 0; g < numarrows; ++g) {
			arrows[i][g].update();
		}
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
		for(int p = 0; p < numplayers; ++p) {
			if(players[i].getGoal().x == pos.x && players[i].getGoal().y == pos.y) {
				empty = false;
			}
			for(int a = 0; a < numarrows; ++a) {
				if(arrows[p][a].isActive()) {
					if(arrows[p][a].getPos().x == pos.x && arrows[p][a].getPos().y == pos.y) {
						empty = false;
					}
				}
			}
		}
		if(empty) {
			for (int g = 4; g < 8; ++g) {
				if (Engine::instance()->getFlags(inputInfo[i][g].c_str())&buttonFlags::_repeat) {
					snapToNextAvailableArrow(i);
					//if (arrows[i][curArrow[i]].isActive())
					//	arrows[i][curArrow[i]].setActive
					arrows[i][curArrow[i]].activate(arrowdecaytime, g - 4, pos);
				}
			}
		}
	}
}


void Match::snapToNextAvailableArrow(int a_player) {
	int startingarrow = curArrow[a_player];
	int temp = startingarrow;
	bool done = false;

	while (!done) {
		if (!arrows[a_player][temp].isActive()) {
			done = true;
		}

		if (!done) {
			temp++;
			if (temp >= numarrows) {
				temp = 0;
			}
			if (temp == startingarrow) {
				done = true;
				temp++;
				if (temp >= numarrows) {
					temp = 0;
				}
			}

		}
	}

	curArrow[a_player] = temp;




}
