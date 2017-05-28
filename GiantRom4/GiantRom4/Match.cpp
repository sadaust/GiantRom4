#include "Match.h"

void Match::init() {
	//p1Binds
	inputInfo[0][0] = "P1MoveUp";
	inputInfo[0][1] = "P1MoveRight";
	inputInfo[0][2] = "P1MoveDown";
	inputInfo[0][3] = "P1MoveLeft";
	inputInfo[0][4] = "P1ArrowUp";
	inputInfo[0][5] = "P1ArrowRight";
	inputInfo[0][6] = "P1ArrowDown";
	inputInfo[0][7] = "P1ArrowLeft";
	//p2Binds
	inputInfo[1][0] = "P2MoveUp";
	inputInfo[1][1] = "P2MoveRight";
	inputInfo[1][2] = "P2MoveDown";
	inputInfo[1][3] = "P2MoveLeft";
	inputInfo[1][4] = "P2ArrowUp";
	inputInfo[1][5] = "P2ArrowRight";
	inputInfo[1][6] = "P2ArrowDown";
	inputInfo[1][7] = "P2ArrowLeft";
	//p3Binds
	inputInfo[2][0] = "P3MoveUp";
	inputInfo[2][1] = "P3MoveRight";
	inputInfo[2][2] = "P3MoveDown";
	inputInfo[2][3] = "P3MoveLeft";
	inputInfo[2][4] = "P3ArrowUp";
	inputInfo[2][5] = "P3ArrowRight";
	inputInfo[2][6] = "P3ArrowDown";
	inputInfo[2][7] = "P3ArrowLeft";
	//p4Binds
	inputInfo[3][0] = "P4MoveUp";
	inputInfo[3][1] = "P4MoveRight";
	inputInfo[3][2] = "P4MoveDown";
	inputInfo[3][3] = "P4MoveLeft";
	inputInfo[3][4] = "P4ArrowUp";
	inputInfo[3][5] = "P4ArrowRight";
	inputInfo[3][6] = "P4ArrowDown";
	inputInfo[3][7] = "P4ArrowLeft";



	Engine::instance()->bind(W,inputInfo[0][0].c_str());
	Engine::instance()->bind(D,inputInfo[0][1].c_str());
	Engine::instance()->bind(S,inputInfo[0][2].c_str());
	Engine::instance()->bind(A,inputInfo[0][3].c_str());
	Engine::instance()->bind(UP,inputInfo[0][4].c_str());
	Engine::instance()->bind(RIGHT,inputInfo[0][5].c_str());
	Engine::instance()->bind(DOWN,inputInfo[0][6].c_str());
	Engine::instance()->bind(LEFT,inputInfo[0][7].c_str());

	//Engine::instance()->bind(pad4_LYp,inputInfo[0][0].c_str());
	//Engine::instance()->bind(pad4_LYn,inputInfo[0][1].c_str());
	//Engine::instance()->bind(pad4_LXn,inputInfo[0][2].c_str());
	//Engine::instance()->bind(pad4_LXp,inputInfo[0][3].c_str());
	//Engine::instance()->bind(pad4_Y,inputInfo[0][4].c_str());
	//Engine::instance()->bind(pad4_A,inputInfo[0][5].c_str());
	//Engine::instance()->bind(pad4_X,inputInfo[0][6].c_str());
	//Engine::instance()->bind(pad4_B,inputInfo[0][7].c_str());

	Engine::instance()->bind(pad1_LYp,inputInfo[1][0].c_str());
	Engine::instance()->bind(pad1_LXp,inputInfo[1][1].c_str());
	Engine::instance()->bind(pad1_LYn,inputInfo[1][2].c_str());
	Engine::instance()->bind(pad1_LXn,inputInfo[1][3].c_str());
	Engine::instance()->bind(pad1_Y,inputInfo[1][4].c_str());
	Engine::instance()->bind(pad1_B,inputInfo[1][5].c_str());
	Engine::instance()->bind(pad1_A,inputInfo[1][6].c_str());
	Engine::instance()->bind(pad1_X,inputInfo[1][7].c_str());

	Engine::instance()->bind(pad2_LYp,inputInfo[2][0].c_str());
	Engine::instance()->bind(pad2_LXp,inputInfo[2][1].c_str());
	Engine::instance()->bind(pad2_LYn,inputInfo[2][2].c_str());
	Engine::instance()->bind(pad2_LXn,inputInfo[2][3].c_str());
	Engine::instance()->bind(pad2_Y,inputInfo[2][4].c_str());
	Engine::instance()->bind(pad2_B,inputInfo[2][5].c_str());
	Engine::instance()->bind(pad2_A,inputInfo[2][6].c_str());
	Engine::instance()->bind(pad2_X,inputInfo[2][7].c_str());

	Engine::instance()->bind(pad3_LYp,inputInfo[3][0].c_str());
	Engine::instance()->bind(pad3_LXp,inputInfo[3][1].c_str());
	Engine::instance()->bind(pad3_LYn,inputInfo[3][2].c_str());
	Engine::instance()->bind(pad3_LXn,inputInfo[3][3].c_str());
	Engine::instance()->bind(pad3_Y,inputInfo[3][4].c_str());
	Engine::instance()->bind(pad3_B,inputInfo[3][5].c_str());
	Engine::instance()->bind(pad3_A,inputInfo[3][6].c_str());
	Engine::instance()->bind(pad3_X,inputInfo[3][7].c_str());

	D3DXCOLOR playerCol[MAXIMUMPLAYERS];

	playerCol[0] = 0xFFFF0000;
	playerCol[1] = 0xFF0000FF;
	playerCol[2] = 0xFF00FF00;
	playerCol[3] = 0xFFF7F83C;

	// make settings for these later
	numplayers = 2;
	numarrows = 3;
	numspawners = 3;
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



	for (int i = 0; i < numspawners; ++i) {
		spawners[i].init();
		spawners[i].setPos(vector(2+(3*i),6,0));
		spawners[i].activate(vector(2 + (3 * i), 6, 0));
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
	for (int i = 0; i < numspawners; ++i) {
		if (spawners[i].isActive()) {
			D3DXMatrixIdentity(&mat);
			D3DXMatrixIdentity(&rot);
			ren = spawners[i].getRen();
			vec = spawners[i].getPos();
			D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f), (gridHeight / 256.0f), 1);
			D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), 0);
			D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &rot);
			D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
			Engine::instance()->addRender(ren);
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
			pos.x += 1;
		}
		if(Engine::instance()->getFlags(inputInfo[i][2].c_str())&buttonFlags::_repeat) {
			pos.y += 1;
		} else if(Engine::instance()->getFlags(inputInfo[i][3].c_str())&buttonFlags::_repeat) {
			pos.x -= 1;
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
		for(int p = 0; p < numplayers && empty; ++p) {
			if(players[i].getGoal().x == pos.x && players[i].getGoal().y == pos.y) {
				empty = false;
			}
			for (int a = 0; a < numarrows && empty; ++a) {
				if(arrows[p][a].isActive()) {
					if(arrows[p][a].getPos().x == pos.x && arrows[p][a].getPos().y == pos.y) {
						empty = false;
					}
				}
			}
		}
		for (int s = 0; s < numspawners && empty; ++s) {
			if (spawners[s].getPos() == pos) {
				empty = false;
			}
		}
		if(empty) {
			for (int g = MAXIMUMPLAYERS; g < MAXIMUMPLAYERS+MAXIMUMPLAYERS; ++g) {
				if (Engine::instance()->getFlags(inputInfo[i][g].c_str())&buttonFlags::_repeat) {
					snapToNextAvailableArrow(i);
					//if (arrows[i][curArrow[i]].isActive())
					//	arrows[i][curArrow[i]].setActive
					arrows[i][curArrow[i]].activate(arrowdecaytime, g - MAXIMUMPLAYERS, pos);
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
