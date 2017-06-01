#pragma once
#include "ArrowMan.h"








void ArrowMan::Init(float a_gridwidth, float a_gridheight, int a_numplayers, int a_numarrows, D3DXCOLOR a_playerColor[MAXIMUMPLAYERS], double a_decaytime) {
	ClearDisSpaces();
	gridWidth = a_gridwidth;
	gridHeight = a_gridheight;
	NumPlayers = a_numplayers;
	NumArrows = a_numarrows;
	for (int i = 0; i < MAXIMUMPLAYERS; ++i) {
		curArrow[i] = 0;
		for (int g = 0; g < NumArrows; ++g) {
			arrows[i][g].init();
			arrows[i][g].setColor(a_playerColor[i]);
		}
	}
	arrowdecaytime = a_decaytime;
}


void ArrowMan::Update() {
	for (int i = 0; i < NumPlayers; ++i) {
		for (int z = 0; z < NumArrows; ++z) {
			arrows[i][z].update();
		}
	}
}


void ArrowMan::Render() {
	renInfo ren;
	vector vec;
	D3DXMATRIX mat;
	D3DXMATRIX rot;
	for (int i = 0; i < NumPlayers; ++i) {
		for (int z = 0; z < NumArrows; ++z) {
			if (arrows[i][z].isActive()) {
				D3DXMatrixIdentity(&mat);
				D3DXMatrixIdentity(&rot);
				ren = arrows[i][z].getRen();
				vec = arrows[i][z].getPos();
				if (arrows[i][z].getDecayTimeLeft() < SMALLARROWTIME)
					D3DXMatrixScaling(&ren.matrix, (gridWidth / 256.0f) * SMALLARROWMULTIPLIER, (gridHeight / 256.0f) * SMALLARROWMULTIPLIER, 1);
				else
					D3DXMatrixScaling(&ren.matrix, gridWidth / 256.0f, gridHeight / 256.0f, 1);
				D3DXMatrixTranslation(&mat, vec.x*gridWidth + (gridWidth*0.5f), vec.y*gridHeight + (gridHeight*0.5f), 0);
				D3DXMatrixRotationZ(&rot, D3DXToRadian(90.0f*arrows[i][z].getRot()));
				D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &rot);
				D3DXMatrixMultiply(&ren.matrix, &ren.matrix, &mat);
				Engine::instance()->addRender(ren);
			}
		}
	}
}


void ArrowMan::AddArrow(int a_player, char a_rot, vector a_pos) {
	bool filled = false; // space is presumed empty until proven filled
	for (int i = 0; i < disallowedspaces.size() && !filled; ++i) {
		if (a_pos == disallowedspaces[i])
			filled = true;
	}
	if (!filled) {
		for (int i = 0; i < NumPlayers && !filled; ++i) {
			for (int z = 0; z < NumArrows && !filled; ++z) {
				if (arrows[i][z].isActive()) {
					if (a_pos == arrows[i][z].getPos())
						filled = true;
				}
			}
		}
	}
	if (!filled) {
		snapToNextAvailableArrow(a_player);
		arrows[a_player][curArrow[a_player]].activate(arrowdecaytime, a_rot, a_pos);
	}
}


void ArrowMan::DisallowSpace(vector a_pos) {
	disallowedspaces.push_back(a_pos);
}


void ArrowMan::ClearDisSpaces() {
	disallowedspaces.clear();
}


void ArrowMan::snapToNextAvailableArrow(int a_player) {
	int startingarrow = curArrow[a_player];
	int temp = startingarrow;
	bool done = false;

	while (!done) {
		if (!arrows[a_player][temp].isActive()) {
			done = true;
		}

		if (!done) {
			temp++;
			if (temp >= NumArrows) {
				temp = 0;
			}
			if (temp == startingarrow) {
				done = true;
				temp++;
				if (temp >= NumArrows) {
					temp = 0;
				}
			}

		}
	}

	curArrow[a_player] = temp;




}




