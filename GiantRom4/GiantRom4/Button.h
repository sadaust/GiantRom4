/*===========================
=Button.h/cpp				=
=by Tyler van Gastel		=
=latest update 11/30/2015	=
=buttons for menus			=
=============================*/
#pragma once
#include <string>
#include "ResourceDefines.h"

class Button {
private:
	void (*function)();
	D3DCOLOR normal, highlight;
	textStruct ren;
public:
	Button();
	Button(void (*func)(),const char* label,frect rect);
	Button(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color);
	Button(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color,D3DCOLOR hcolor);
	void setButton(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color,D3DCOLOR hcolor);
	void setRen(DWORD flags,D3DCOLOR color);
	void setFunc(void* func);
	//checks if cords are withing the button
	void click(float mouseX,float mouseY);
	bool mouseHover(float mouseX,float mouseY);
	//click without checking cords
	void click();
	void render(bool highlight);



	// added by jersh
	// change color
	void changecolor(D3DCOLOR color, D3DCOLOR hcolor);
};