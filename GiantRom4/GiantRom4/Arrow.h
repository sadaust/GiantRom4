#pragma once
#include "Engine.h"

class Arrow {
private:
	vector pos;
	char rot;
	spriteStruct spri;
	D3DXCOLOR color;
	bool active;
	double decaytimeleft;
	double lasttimeupdated;
public:
	Arrow();
	void init();
	void activate(double decaytime, char a_rot, vector a_pos);
	bool isActive();
	char getRot();
	vector getPos();
	double getDecayTimeLeft();
	void update();
	void setLoc(char a_rot,vector a_pos);
	void setActive(bool aActive) {active = aActive;}
	void setColor(D3DXCOLOR col);
	void setDecayTime(double a_decaytime);
	renInfo getRen();
};