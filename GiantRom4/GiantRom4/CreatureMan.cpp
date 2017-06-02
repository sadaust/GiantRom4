#include "CreatureMan.h"




CreatureMan::CreatureMan() {
	minTimeBetweenSpawns = DEFAULTMINSPAWNTIME;
	maxTimeBetweenSpawns = DEFAULTMAXSPAWNTIME;
	speedmultiplier = 1.0f;



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
}


void CreatureMan::Init(float a_gridwidth, float a_gridheight, float a_minTimeBetweenSpawns, float a_maxTimeBetweenSpawns, float a_speedmultiplier) {
	gridWidth = a_gridwidth;
	gridHeight = a_gridheight;

	minTimeBetweenSpawns = a_minTimeBetweenSpawns;
	maxTimeBetweenSpawns = a_maxTimeBetweenSpawns;

	spawners.push_back(Spawner(vector(7, 5, 0), 0, true));
	spawners[0].setNewTimeToSpawn(minTimeBetweenSpawns,maxTimeBetweenSpawns);
	spawners.push_back(Spawner(vector(1, 1, 0), 2, true));
	spawners[1].setNewTimeToSpawn(minTimeBetweenSpawns, maxTimeBetweenSpawns);
	spawners.push_back(Spawner(vector(9, 1, 0), 3, true));
	spawners[2].setNewTimeToSpawn(minTimeBetweenSpawns, maxTimeBetweenSpawns);

	speedmultiplier = a_speedmultiplier;
}


void CreatureMan::Update() {
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
}


void CreatureMan::Render() {
	renInfo ren;
	vector vec;
	D3DXMATRIX mat;
	//for (int i = 0; i < spawners.size(); ++i) {
	//	if (spawners[i].isActive()) {
	//		ren.type = screenSprite;
	//		D3DXMatrixIdentity(&mat);
	//		D3DXMatrixIdentity(&ren.matrix);
	//		ren.asset = &spawnersprite;
	//		vec = spawners[i].getPos();
	//		D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f), (gridHeight / 256.0f), 1);
	//		D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), 0);
	//		D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
	//		Engine::instance()->addRender(ren);
	//	}
	//}
	//for (int i = 0; i < creatures.size(); ++i) {
	//	if (creatures[i].isActive()) {
	//		D3DXMatrixIdentity(&mat);
	//		ren.asset = &creaturesprites[creatures[i].getType()][creatures[i].getDir()];
	//		ren.type = screenSprite;
	//		D3DXMatrixIdentity(&ren.matrix);
	//		vec = creatures[i].getPos();
	//		D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f), (gridHeight / 256.0f), 1);
	//		D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), 0);
	//		D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
	//		Engine::instance()->addRender(ren);
	//	}
	//}
}


void CreatureMan::setSpeed(float a_speed) {
	speedmultiplier = a_speed;
}


void CreatureMan::addSpawner(Spawner a_spawn) {
	spawners.push_back(a_spawn);
}


void CreatureMan::ClearCreatures() {
	creatures.clear();
}


void CreatureMan::ClearSpawners() {
	spawners.clear();
	timetospawn.clear();
}



