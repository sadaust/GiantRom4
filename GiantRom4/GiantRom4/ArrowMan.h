#pragma once
#include "Arrow.h"
#include "ChompDefines.h"












class ArrowMan {
private:
	float gridWidth;
	float gridHeight;
	int curArrow[MAXIMUMPLAYERS];
	Arrow arrows[MAXIMUMPLAYERS][MAXIMUMARROWS];
	int NumPlayers;
	int NumArrows; // number of arrows that are available in the game mode.
	double arrowdecaytime;
	std::vector <vector> disallowedspaces; // vector vector lol

	void snapToNextAvailableArrow(int a_player);
	void ClearDisSpaces();
public:
	void Init(float a_gridwidth, float a_gridheight, int a_numplayers, int a_numarrows, D3DXCOLOR a_playerColor[MAXIMUMPLAYERS], double a_decaytime);
	void Update();
	void Render();
	void AddArrow(int a_player, char a_rot, vector a_pos);
	void DisallowSpace(vector a_pos);
};