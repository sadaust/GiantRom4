#pragma once
#include "Engine.h"

class Player {
private:
	int wins;
	int points;
	vector goalPos;
	vector cPos;
	spriteStruct cursor;
	spriteStruct base;
	D3DXCOLOR color;
public:
	void init();
	int getScore() {return points;}
	void resetScore() {points = 0;}
	int addScore(int i) {points += i;}
	void resetWins() {wins = 0;}
	int getWins() {return wins;}
	//wins += 1
	int win() {wins += 1;}
	void setGoal(int x, int y) {goalPos.x = x; goalPos.y = y; goalPos.z = 0;}
	vector getGoal() {return goalPos;}
	D3DXCOLOR getColor();
	void setColor(D3DXCOLOR pColor) {color = pColor; cursor.color = color;base.color = color;}
	renInfo getRen();
	renInfo baseRen();
	void setCur(vector pos) {cPos = pos;}
	vector getCur() {return cPos;};
};