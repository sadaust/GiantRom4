/*===========================
=MenuSystem.h/cpp			=
=by Tyler van Gastel		=
=latest update 08/07/2016	=
=buttons for menus			=
=============================*/

#pragma once
#include "Engine.h"
#include "Button.h"

class MenuSystem {
private:
	int selection;
	std::vector<Button> buttons;
	bool hold;
	//mouse stuff
	float _mX;
	float _mY;
	int _height;
	int _width;
	spriteStruct cursor;

	// sound stuff
	soundStruct click;
	soundStruct rollover;
	vector soundvec;
public:
	MenuSystem();
	void init();
	void update();
	void clear();
	void render();
	void resetSelection(){selection = 0;}
	void addButton(Button input);
	void addButton(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color,D3DCOLOR hcolor);



	// this is gross
	void ChangeColorJanky(int index, D3DCOLOR color, D3DCOLOR hcolor);
	// more grossness
	int numButtons();
};