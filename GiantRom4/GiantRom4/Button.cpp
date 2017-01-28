#include "Button.h"
#include "Engine.h"

void NOFUNC() {

}

Button::Button() {
	function = NOFUNC;
	ren.rect.bottom = 1.0f;
	ren.rect.top = 0;
	ren.rect.left = 0;
	ren.rect.right = 1.0f;
	ren.text = "ERROR";
	ren.flags = DT_TOP | DT_LEFT | DT_NOCLIP;
	ren.color = 0xFFFFFFFF;
	normal = 0xFFFFFFFF;
	highlight = 0xFFFFFFFF;
}

Button::Button(void (*func)(),const char* label,frect rect) {
	function = func;
	ren.text = label;
	ren.rect = rect;
	ren.flags = DT_VCENTER | DT_LEFT;
	ren.color = 0xFFFFFFFF;
	normal = 0xFFFFFFFF;
	highlight = 0xFFFFFFFF;
}

Button::Button(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color) {
	function = func;
	ren.text = label;
	ren.rect = rect;
	ren.flags = flags;
	ren.color = color;
	normal = color;
	highlight = 0xFFFFFFFF;
}

Button::Button(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color,D3DCOLOR hcolor) {
	function = func;
	ren.text = label;
	ren.rect = rect;
	ren.flags = flags;
	ren.color = color;
	normal = color;
	highlight = hcolor;
}

void Button::setButton(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color,D3DCOLOR hcolor) {
	function = func;
	ren.text = label;
	ren.rect = rect;
	ren.flags = flags;
	ren.color = color;
	normal = color;
	highlight = hcolor;
}

void Button::click(float mouseX,float mouseY) {
	if(ren.rect.bottom >= mouseY && mouseY >= ren.rect.top && mouseX >= ren.rect.left && mouseX <= ren.rect.right) {
		function();
	}
}

bool Button::mouseHover(float mouseX,float mouseY) {
	if(ren.rect.bottom >= mouseY && mouseY >= ren.rect.top && mouseX >= ren.rect.left && mouseX <= ren.rect.right) {
		return true;
	}
	return false;
}

void Button::click() {
	function();
}

void Button::render(bool bhighlight) {
	renInfo temp;

	if(bhighlight) {
		ren.color = highlight;
	} else {
		ren.color = normal;
	}

	temp.type = text;
	temp.asset = &ren;
	D3DXMatrixIdentity(&temp.matrix);
	D3DXMatrixTranslation(&temp.matrix,0,0,0.9);
	Engine::instance()->addRender(temp);
}


// Jersh added this
void Button::changecolor(D3DCOLOR color, D3DCOLOR hcolor) {
	normal = color;
	highlight = hcolor;
}
