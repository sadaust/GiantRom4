#include "GameplayMan.h"




GameplayMan::GameplayMan() {
	minTimeBetweenSpawns = DEFAULTMINSPAWNTIME;
	maxTimeBetweenSpawns = DEFAULTMAXSPAWNTIME;
	speedmultiplier = 1.0f;


}


void GameplayMan::Init(float a_gridwidth, float a_gridheight, int a_numplayers, int a_numarrows, double a_decaytime, float a_minTimeBetweenSpawns, float a_maxTimeBetweenSpawns, float a_speedmultiplier, int a_maxActiveArrows) {
	gridWidth = a_gridwidth;
	gridHeight = a_gridheight;


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

	// debug creature spawning buttons
	debugInputInfo[0] = "CreatureUp";
	debugInputInfo[1] = "CreatureRight";
	debugInputInfo[2] = "CreatureDown";
	debugInputInfo[3] = "CreatureLeft";

	Engine::instance()->bind(NP_8, debugInputInfo[0].c_str());
	Engine::instance()->bind(NP_6, debugInputInfo[1].c_str());
	Engine::instance()->bind(NP_2, debugInputInfo[2].c_str());
	Engine::instance()->bind(NP_4, debugInputInfo[3].c_str());


	Engine::instance()->bind(W, inputInfo[0][0].c_str());
	Engine::instance()->bind(D, inputInfo[0][1].c_str());
	Engine::instance()->bind(S, inputInfo[0][2].c_str());
	Engine::instance()->bind(A, inputInfo[0][3].c_str());
	Engine::instance()->bind(UP, inputInfo[0][4].c_str());
	Engine::instance()->bind(RIGHT, inputInfo[0][5].c_str());
	Engine::instance()->bind(DOWN, inputInfo[0][6].c_str());
	Engine::instance()->bind(LEFT, inputInfo[0][7].c_str());

	//Engine::instance()->bind(pad4_LYp,inputInfo[0][0].c_str());
	//Engine::instance()->bind(pad4_LYn,inputInfo[0][1].c_str());
	//Engine::instance()->bind(pad4_LXn,inputInfo[0][2].c_str());
	//Engine::instance()->bind(pad4_LXp,inputInfo[0][3].c_str());
	//Engine::instance()->bind(pad4_Y,inputInfo[0][4].c_str());
	//Engine::instance()->bind(pad4_A,inputInfo[0][5].c_str());
	//Engine::instance()->bind(pad4_X,inputInfo[0][6].c_str());
	//Engine::instance()->bind(pad4_B,inputInfo[0][7].c_str());

	Engine::instance()->bind(pad1_LYp, inputInfo[1][0].c_str());
	Engine::instance()->bind(pad1_LXp, inputInfo[1][1].c_str());
	Engine::instance()->bind(pad1_LYn, inputInfo[1][2].c_str());
	Engine::instance()->bind(pad1_LXn, inputInfo[1][3].c_str());
	Engine::instance()->bind(pad1_Y, inputInfo[1][4].c_str());
	Engine::instance()->bind(pad1_B, inputInfo[1][5].c_str());
	Engine::instance()->bind(pad1_A, inputInfo[1][6].c_str());
	Engine::instance()->bind(pad1_X, inputInfo[1][7].c_str());

	Engine::instance()->bind(pad2_LYp, inputInfo[2][0].c_str());
	Engine::instance()->bind(pad2_LXp, inputInfo[2][1].c_str());
	Engine::instance()->bind(pad2_LYn, inputInfo[2][2].c_str());
	Engine::instance()->bind(pad2_LXn, inputInfo[2][3].c_str());
	Engine::instance()->bind(pad2_Y, inputInfo[2][4].c_str());
	Engine::instance()->bind(pad2_B, inputInfo[2][5].c_str());
	Engine::instance()->bind(pad2_A, inputInfo[2][6].c_str());
	Engine::instance()->bind(pad2_X, inputInfo[2][7].c_str());

	Engine::instance()->bind(pad3_LYp, inputInfo[3][0].c_str());
	Engine::instance()->bind(pad3_LXp, inputInfo[3][1].c_str());
	Engine::instance()->bind(pad3_LYn, inputInfo[3][2].c_str());
	Engine::instance()->bind(pad3_LXn, inputInfo[3][3].c_str());
	Engine::instance()->bind(pad3_Y, inputInfo[3][4].c_str());
	Engine::instance()->bind(pad3_B, inputInfo[3][5].c_str());
	Engine::instance()->bind(pad3_A, inputInfo[3][6].c_str());
	Engine::instance()->bind(pad3_X, inputInfo[3][7].c_str());



	D3DXCOLOR playerCol[MAXIMUMPLAYERS];

	playerCol[0] = 0xFFFF0000;
	playerCol[1] = 0xFF0000FF;
	playerCol[2] = 0xFF00FF00;
	playerCol[3] = 0xFFF7F83C;


	NumPlayers = 2;
	NumArrows = 3;
	for (int i = 0; i < NumPlayers; ++i) {
		players[i].init();
		players[i].setColor(playerCol[i]);
		players[i].setCur(vector(3 + (5 * i), 4, 0));
		players[i].setGoal(3 + (5 * i), 4);
	}

	//arrowman.Init(gridWidth, gridHeight, numplayers, numarrows, playerCol, arrowdecaytime);








	gridWidth = a_gridwidth;
	gridHeight = a_gridheight;
	maxActiveArrows = a_maxActiveArrows;
	for (int i = 0; i < MAXIMUMPLAYERS; ++i) {
		curArrow[i] = 0;
		for (int g = 0; g < maxActiveArrows; ++g) {
			arrows.push_back(Arrow(SMALLARROWTIME * 2, 0, vector(0, 0, 0), i, players[i].getColor()));
		}
	}
	arrowdecaytime = a_decaytime;

	minTimeBetweenSpawns = a_minTimeBetweenSpawns;
	maxTimeBetweenSpawns = a_maxTimeBetweenSpawns;

	spawners.push_back(Spawner(vector(5, 5, 0), 0, true));
	spawners[0].setNewTimeToSpawn(minTimeBetweenSpawns,maxTimeBetweenSpawns);
	spawners.push_back(Spawner(vector(1, 1, 0), 1, true));
	spawners[1].setNewTimeToSpawn(minTimeBetweenSpawns, maxTimeBetweenSpawns);
	spawners.push_back(Spawner(vector(9, 1, 0), 3, true));
	spawners[2].setNewTimeToSpawn(minTimeBetweenSpawns, maxTimeBetweenSpawns);

	walls.push_back(Wall(vector(1, 1, 0), false));
	walls.push_back(Wall(vector(2, 1, 0), true));
	walls.push_back(Wall(vector(3, 1, 0), true));
	walls.push_back(Wall(vector(4, 1, 0), true));
	walls.push_back(Wall(vector(5, 1, 0), true));
	walls.push_back(Wall(vector(6, 1, 0), false));
	walls.push_back(Wall(vector(8, 1, 0), false));
	walls.push_back(Wall(vector(7, 2, 0), true));
	walls.push_back(Wall(vector(5, 5, 0), true));
	walls.push_back(Wall(vector(1, 5, 0), true));
	walls.push_back(Wall(vector(0, 5, 0), false));
	walls.push_back(Wall(vector(8, 0, 0), false));
	walls.push_back(Wall(vector(6, 1, 0), true));









	speedmultiplier = a_speedmultiplier;
	creaturesprites[0][0].image = (imageAsset*)((Engine::instance()->getResource("pupup.png", 0xFF000000))->resource);
	creaturesprites[0][0].rec.left = 0;
	creaturesprites[0][0].rec.top = 0;
	creaturesprites[0][0].rec.right = creaturesprites[0][0].image->texInfo.Width;
	creaturesprites[0][0].rec.bottom = creaturesprites[0][0].image->texInfo.Height;
	creaturesprites[0][0].center.x = creaturesprites[0][0].rec.right / 2;
	creaturesprites[0][0].center.y = creaturesprites[0][0].rec.bottom / 2;
	creaturesprites[0][0].center.z = 0;
	creaturesprites[0][0].color = 0xFFFFFFFF;
	creaturesprites[0][1].image = (imageAsset*)((Engine::instance()->getResource("pupright.png", 0xFF000000))->resource);
	creaturesprites[0][1].rec.left = 0;
	creaturesprites[0][1].rec.top = 0;
	creaturesprites[0][1].rec.right = creaturesprites[0][1].image->texInfo.Width;
	creaturesprites[0][1].rec.bottom = creaturesprites[0][1].image->texInfo.Height;
	creaturesprites[0][1].center.x = creaturesprites[0][1].rec.right / 2;
	creaturesprites[0][1].center.y = creaturesprites[0][1].rec.bottom / 2;
	creaturesprites[0][1].center.z = 0;
	creaturesprites[0][1].color = 0xFFFFFFFF;
	creaturesprites[0][2].image = (imageAsset*)((Engine::instance()->getResource("pupdown.png", 0xFF000000))->resource);
	creaturesprites[0][2].rec.left = 0;
	creaturesprites[0][2].rec.top = 0;
	creaturesprites[0][2].rec.right = creaturesprites[0][2].image->texInfo.Width;
	creaturesprites[0][2].rec.bottom = creaturesprites[0][2].image->texInfo.Height;
	creaturesprites[0][2].center.x = creaturesprites[0][2].rec.right / 2;
	creaturesprites[0][2].center.y = creaturesprites[0][2].rec.bottom / 2;
	creaturesprites[0][2].center.z = 0;
	creaturesprites[0][2].color = 0xFFFFFFFF;
	creaturesprites[0][3].image = (imageAsset*)((Engine::instance()->getResource("pupleft.png", 0xFF000000))->resource);
	creaturesprites[0][3].rec.left = 0;
	creaturesprites[0][3].rec.top = 0;
	creaturesprites[0][3].rec.right = creaturesprites[0][3].image->texInfo.Width;
	creaturesprites[0][3].rec.bottom = creaturesprites[0][3].image->texInfo.Height;
	creaturesprites[0][3].center.x = creaturesprites[0][3].rec.right / 2;
	creaturesprites[0][3].center.y = creaturesprites[0][3].rec.bottom / 2;
	creaturesprites[0][3].center.z = 0;
	creaturesprites[0][3].color = 0xFFFFFFFF;
	creaturesprites[1][0].image = (imageAsset*)((Engine::instance()->getResource("rorieup.png", 0xFF000000))->resource);
	creaturesprites[1][0].rec.left = 0;
	creaturesprites[1][0].rec.top = 0;
	creaturesprites[1][0].rec.right = creaturesprites[1][0].image->texInfo.Width;
	creaturesprites[1][0].rec.bottom = creaturesprites[1][0].image->texInfo.Height;
	creaturesprites[1][0].center.x = creaturesprites[1][0].rec.right / 2;
	creaturesprites[1][0].center.y = creaturesprites[1][0].rec.bottom / 2;
	creaturesprites[1][0].center.z = 0;
	creaturesprites[1][0].color = 0xFFFFFFFF;
	creaturesprites[1][1].image = (imageAsset*)((Engine::instance()->getResource("rorieright.png", 0xFF000000))->resource);
	creaturesprites[1][1].rec.left = 0;
	creaturesprites[1][1].rec.top = 0;
	creaturesprites[1][1].rec.right = creaturesprites[1][1].image->texInfo.Width;
	creaturesprites[1][1].rec.bottom = creaturesprites[1][1].image->texInfo.Height;
	creaturesprites[1][1].center.x = creaturesprites[1][1].rec.right / 2;
	creaturesprites[1][1].center.y = creaturesprites[1][1].rec.bottom / 2;
	creaturesprites[1][1].center.z = 0;
	creaturesprites[1][1].color = 0xFFFFFFFF;
	creaturesprites[1][2].image = (imageAsset*)((Engine::instance()->getResource("roriedown.png", 0xFF000000))->resource);
	creaturesprites[1][2].rec.left = 0;
	creaturesprites[1][2].rec.top = 0;
	creaturesprites[1][2].rec.right = creaturesprites[1][2].image->texInfo.Width;
	creaturesprites[1][2].rec.bottom = creaturesprites[1][2].image->texInfo.Height;
	creaturesprites[1][2].center.x = creaturesprites[1][2].rec.right / 2;
	creaturesprites[1][2].center.y = creaturesprites[1][2].rec.bottom / 2;
	creaturesprites[1][2].center.z = 0;
	creaturesprites[1][2].color = 0xFFFFFFFF;
	creaturesprites[1][3].image = (imageAsset*)((Engine::instance()->getResource("rorieleft.png", 0xFF000000))->resource);
	creaturesprites[1][3].rec.left = 0;
	creaturesprites[1][3].rec.top = 0;
	creaturesprites[1][3].rec.right = creaturesprites[1][3].image->texInfo.Width;
	creaturesprites[1][3].rec.bottom = creaturesprites[1][3].image->texInfo.Height;
	creaturesprites[1][3].center.x = creaturesprites[1][3].rec.right / 2;
	creaturesprites[1][3].center.y = creaturesprites[1][3].rec.bottom / 2;
	creaturesprites[1][3].center.z = 0;
	creaturesprites[1][3].color = 0xFFFFFFFF;




	spawnersprite.image = (imageAsset*)((Engine::instance()->getResource("spawner.png", 0xFF000000))->resource);
	spawnersprite.rec.left = 0;
	spawnersprite.rec.top = 0;
	spawnersprite.rec.right = spawnersprite.image->texInfo.Width;
	spawnersprite.rec.bottom = spawnersprite.image->texInfo.Height;
	spawnersprite.center.x = spawnersprite.rec.right / 2;
	spawnersprite.center.y = spawnersprite.rec.bottom / 2;
	spawnersprite.center.z = 0;
	spawnersprite.color = 0xFFFFFFFF;




	wallsprite.image = (imageAsset*)((Engine::instance()->getResource("wall.png", 0xFF000000))->resource);
	wallsprite.rec.left = 0;
	wallsprite.rec.top = 0;
	wallsprite.rec.right = wallsprite.image->texInfo.Width;
	wallsprite.rec.bottom = wallsprite.image->texInfo.Height;
	wallsprite.center.x = wallsprite.rec.right / 2;
	wallsprite.center.y = wallsprite.rec.bottom / 2;
	wallsprite.center.z = 0;
	wallsprite.color = 0xFFFFFFFF;





	for (int i = 0; i < NumPlayers; ++i) {
		arrowsprite[i].image = (imageAsset*)((Engine::instance()->getResource("arrow.png", 0xFF000000))->resource);
		arrowsprite[i].rec.left = 0;
		arrowsprite[i].rec.top = 0;
		arrowsprite[i].rec.right = arrowsprite[i].image->texInfo.Width;
		arrowsprite[i].rec.bottom = arrowsprite[i].image->texInfo.Height;
		arrowsprite[i].center.x = arrowsprite[i].rec.right / 2;
		arrowsprite[i].center.y = arrowsprite[i].rec.bottom / 2;
		arrowsprite[i].center.z = 0;
		arrowsprite[i].color = players[i].getColor();
	}
}


void GameplayMan::Update() {
	vector pos;
	bool empty = true;
	for (int i = 0; i < NumPlayers; ++i) {
		//for (int g = 0; g < numarrows; ++g) {
		//	arrows[i][g].update();
		//}
		pos = players[i].getCur();
		if (Engine::instance()->getFlags(inputInfo[i][0].c_str())&buttonFlags::_repeat) {
			pos.y -= 1;
		}
		else if (Engine::instance()->getFlags(inputInfo[i][1].c_str())&buttonFlags::_repeat) {
			pos.x += 1;
		}
		if (Engine::instance()->getFlags(inputInfo[i][2].c_str())&buttonFlags::_repeat) {
			pos.y += 1;
		}
		else if (Engine::instance()->getFlags(inputInfo[i][3].c_str())&buttonFlags::_repeat) {
			pos.x -= 1;
		}

		if (pos.y > MAPSIZE_Y) {
			pos.y = MAPSIZE_Y;
		}
		else if (pos.y < 0) {
			pos.y = 0;
		}

		if (pos.x >= MAPSIZE_X) {
			pos.x = MAPSIZE_X - 1;
		}
		else if (pos.x < 0) {
			pos.x = 0;
		}
		players[i].setCur(pos);
		empty = true;
		for (int g = MAXIMUMPLAYERS; g < MAXIMUMPLAYERS + MAXIMUMPLAYERS; ++g) {
			if (Engine::instance()->getFlags(inputInfo[i][g].c_str())&buttonFlags::_repeat) {
				AddArrow(i, g-MAXIMUMPLAYERS, players[i].getCur());
			}
		}
	}
	// debug creature spawning
	for (int i = 0; i < 4; ++i) {
		if (Engine::instance()->getFlags(debugInputInfo[i].c_str())&buttonFlags::_repeat) {
			creatures.push_back(Creature());
		}
	}


	for (int i = 0; i < arrows.size(); ++i) {
		arrows[i].update();
	}
	for (int i = 0; i < spawners.size(); ++i) {
		if (spawners[i].isActive()) {
			spawners[i].Update();
			if (spawners[i].doISpawn()) {
				creatures.push_back(Creature(0, spawners[i].getDir(), spawners[i].getPos()));
				spawners[i].resetDoISpawn();
				spawners[i].setNewTimeToSpawn(minTimeBetweenSpawns,maxTimeBetweenSpawns);
			}
		}
	}
	for (int i = 0; i < creatures.size(); ++i) {
		if (creatures[i].doICrossTheLine(creatures[i].distanceToTravel(speedmultiplier))) {
			creatures[i].update(speedmultiplier, walls, arrows);
		}
		else {
			creatures[i].update(speedmultiplier);
		}
	}
}


void GameplayMan::Render() {
	renInfo ren;
	vector vec;
	D3DXMATRIX mat, rot;
	for (int i = 0; i < NumPlayers; ++i) {
		D3DXMatrixIdentity(&mat);
		D3DXMatrixIdentity(&ren.matrix);
		ren = players[i].getRen();
		vec = players[i].getCur();
		D3DXMatrixScaling(&ren.matrix, gridWidth / 256.0f, gridHeight / 256.0f, 1);
		D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), vec.z);
		D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
		Engine::instance()->addRender(ren);
		D3DXMatrixIdentity(&mat);
		D3DXMatrixIdentity(&ren.matrix);
		ren = players[i].baseRen();
		vec = players[i].getGoal();
		D3DXMatrixScaling(&ren.matrix, gridWidth / 256.0f, gridHeight / 256.0f, 1);
		D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), vec.z);
		D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
		Engine::instance()->addRender(ren);
	}
	for (int i = 0; i < arrows.size(); ++i) {
		if (arrows[i].isActive()) {
				D3DXMatrixIdentity(&mat);
				D3DXMatrixIdentity(&rot);
				ren.asset = &arrowsprite[arrows[i].getPlayer()];
				D3DXMatrixIdentity(&ren.matrix);
				vec = arrows[i].getPos();
				if (arrows[i].getDecayTimeLeft() < SMALLARROWTIME)
					D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f) * SMALLARROWMULTIPLIER, (gridHeight / 256.0f) * SMALLARROWMULTIPLIER, 1);
				else
					D3DXMatrixScaling(&ren.matrix, gridWidth / 256.0f, gridHeight / 256.0f, 1);
				D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), 0);
				D3DXMatrixRotationZ(&rot, D3DXToRadian(90.0f*arrows[i].getRot()));
				D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &rot);
				D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
				Engine::instance()->addRender(ren);
		}
	}
	for (int i = 0; i < walls.size(); ++i) {
		ren.type = screenSprite;
		D3DXMatrixIdentity(&mat);
		D3DXMatrixIdentity(&rot);
		D3DXMatrixIdentity(&ren.matrix);
		ren.asset = &wallsprite;
		vec = walls[i].getPos();
		if (!walls[i].amIHorizontal()) {
			D3DXMatrixScaling(&ren.matrix, (gridHeight / 256.0f), (gridWidth / 256.0f), 1);
			D3DXMatrixRotationZ(&rot, D3DXToRadian(90.0f));
			D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &rot);
			D3DXMatrixTranslation(&mat, vec.x*gridWidth/* + (gridWidth*0.5f)*/, vec.y*gridHeight + (gridHeight*0.5f), 0);
		}
		else {
			D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f), (gridHeight / 256.0f), 1);
			D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight/* + (gridHeight*0.5f)*/, 0);
		}
		D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
		Engine::instance()->addRender(ren);
	}
	for (int i = 0; i < spawners.size(); ++i) {
		if (spawners[i].isActive()) {
			ren.type = screenSprite;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixIdentity(&ren.matrix);
			ren.asset = &spawnersprite;
			vec = spawners[i].getPos();
			D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f), (gridHeight / 256.0f), 1);
			D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), 0);
			D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
			Engine::instance()->addRender(ren);
		}
	}
	for (int i = 0; i < creatures.size(); ++i) {
		if (creatures[i].isActive()) {
			D3DXMatrixIdentity(&mat);
			ren.asset = &creaturesprites[creatures[i].getType()][creatures[i].getDir()];
			ren.type = screenSprite;
			D3DXMatrixIdentity(&ren.matrix);
			vec = creatures[i].getPos();
			D3DXMatrixScaling(&ren.matrix, SMALLPUPMULTIPLIER * (gridWidth / 256.0f), SMALLPUPMULTIPLIER * (gridHeight / 256.0f), 1);
			D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), 0);
			D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
			Engine::instance()->addRender(ren);
		}
	}
}


void GameplayMan::setSpeed(float a_speed) {
	speedmultiplier = a_speed;
}


void GameplayMan::addSpawner(Spawner a_spawn) {
	spawners.push_back(a_spawn);
}


void GameplayMan::AddArrow(int a_player, char a_rot, vector a_pos) {
	bool filled = false; // space is presumed empty until proven filled
	int nextArrow = 0;
	if (FindNextArrow(a_player, nextArrow)) {
		for (int i = 0; i < NumPlayers && !filled; ++i) {
			if (a_pos == players[i].getGoal())
				filled = true;
		}
		if (!filled) {
			for (int i = 0; i < spawners.size(); ++i) {
				if (a_pos == spawners[i].getPos())
					filled = true;
			}
		}
		if (!filled) {
			for (int i = 0; i < arrows.size() && !filled; ++i) {
				if (arrows[i].isActive()) {
					if (a_pos == arrows[i].getPos())
						filled = true;
				}

			}
		}
		if (!filled) {
			arrows[nextArrow].activate(arrowdecaytime, a_rot, a_pos, a_player);
		}
	}
}


void GameplayMan::ClearCreatures() {
	creatures.clear();
}


void GameplayMan::ClearSpawners() {
	spawners.clear();
	timetospawn.clear();
}


bool GameplayMan::FindNextArrow(int a_player, int & a_output) {
	for (int i = maxActiveArrows*a_player; i < maxActiveArrows*(a_player+1); ++i) {
		if (!arrows[i].isActive()) {
			a_output = i;
			return true;
		}
	}
	return false;
}





