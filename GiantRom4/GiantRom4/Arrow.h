#pragma once
#include "Engine.h"

class Arrow {
private:
	vector pos;
	char rot;
	int player;
	D3DXCOLOR color;
	bool active;
	double decaytimeleft;
public:
	Arrow();
	Arrow(double a_decaytime, char a_rot, vector a_pos, int a_player, D3DXCOLOR a_color);
	void init();
	void init(double a_decaytime, char a_rot, vector a_pos, int a_player, D3DXCOLOR a_color);
	void activate(double decaytime, char a_rot, vector a_pos, int a_player);
	bool isActive();
	int getPlayer();
	char getRot();
	vector getPos();
	double getDecayTimeLeft();
	void update();
	void setPlayer(int a_player);
	void setLoc(char a_rot,vector a_pos);
	void setActive(bool aActive) {active = aActive;}
	void setColor(D3DXCOLOR a_col);
	void setDecayTime(double a_decaytime);
};