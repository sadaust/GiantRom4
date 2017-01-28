#include "MenuSystem.h"

MenuSystem::MenuSystem() {
	hold = false;
	selection = -1;
	cursor.image = 0;
}

void MenuSystem::init() {
	//click = *(soundStruct*)Engine::instance()->getResource("click.ogg", audio)->resource;
	//rollover = *(soundStruct*)Engine::instance()->getResource("rollover.ogg", audio)->resource;
	soundvec.x = 0;
	soundvec.y = 0;
	soundvec.z = 0;

	_mX = Engine::instance()->getWidth()/2.0f;
	_mY = Engine::instance()->getHeight()/2.0f;
	_height = Engine::instance()->getHeight();
	_width = Engine::instance()->getWidth();
	cursor.image = (imageAsset*)(Engine::instance()->getResource("Cursor.png",D3DXCOLOR(0,0,0,255))->resource);
	cursor.color = D3DXCOLOR(255,255,255,255);
	cursor.center = D3DXVECTOR3(0,0,0);
	cursor.rec.top = 0;
	cursor.rec.left = 0;
	cursor.rec.right = cursor.image->texInfo.Width;
	cursor.rec.bottom = cursor.image->texInfo.Height;
}


void MenuSystem::clear() {
	selection = -1;
	while(buttons.size())
		buttons.pop_back();
}

void MenuSystem::update() {
	_mX += Engine::instance()->getAxis("MouseX");
	_mY += Engine::instance()->getAxis("MouseY");
	if(_mX < 0) {
		_mX = 0;
	} else if(_mX > _width) {
		_mX = _width;
	}
	if(_mY < 0) {
		_mY = 0;
	} else if(_mY > _height) {
		_mY = _height;
	}

	float sx = 0;
	float sy = 0;
	sx = _mX / (float)_width;
	sy = _mY / (float)_height;
	if(buttons.size() > 0) {
		if(Engine::instance()->getAxis("MouseX") != 0 || Engine::instance()->getAxis("MouseY") != 0){
			selection = -1;
			for(int i = 0; i < buttons.size(); ++i) {
				if(buttons[i].mouseHover(sx,sy)) {
					selection = i;
					break;
				}
			}
		} else {
			if(Engine::instance()->getFlags("SelectionUp")& buttonFlags::_repeat) {
				if(!hold) {
					--selection;
					hold = true;
					//Engine::instance()->playSound(rollover, soundvec, soundvec);
				}
				if(selection < 0) {
					selection = buttons.size()-1;
				} else if(selection >= buttons.size()) {
					selection = 0;
				}
			} else if(Engine::instance()->getFlags("SelectionDown")& buttonFlags::_repeat) {
				if(!hold) {
					++selection;
					hold = true;
					//Engine::instance()->playSound(rollover, soundvec, soundvec);
				}
				if(selection < 0) {
					selection = buttons.size()-1;
				} else if(selection >= buttons.size()) {
					selection = 0;
				}
			}

		} 
		if(Engine::instance()->getFlags("Accept")& buttonFlags::_repeat) {
			if(!hold) {
				hold = true;
				if(selection >= 0 && selection < buttons.size()) {
					buttons[selection].click();
					//Engine::instance()->playSound(click, soundvec, soundvec);
				}
			}
		} else {
			hold = false;
		}
	}
}

void MenuSystem::render() {
	renInfo temp;
	D3DXMATRIX mat;
	for(unsigned int i = 0; i < buttons.size(); ++i) {
		buttons[i].render(i == selection);
	}
	temp.type = screenSprite;
	temp.asset = &cursor;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixTranslation(&mat,_mX,_mY,0);
	temp.matrix = mat;
	Engine::instance()->addRender(temp);
}

void MenuSystem::addButton(Button in) {
	buttons.push_back(in);
}

void MenuSystem::addButton(void (*func)(),const char* label,frect rect,DWORD flags,D3DCOLOR color,D3DCOLOR hcolor) {
	Button temp;
	temp.setButton(func,label,rect,flags,color,hcolor);
	buttons.push_back(temp);
}


//ew
void MenuSystem::ChangeColorJanky(int index, D3DCOLOR color, D3DCOLOR hcolor) {
	buttons[index].changecolor(color, hcolor);
}

// more ew
int MenuSystem::numButtons() {
	return buttons.size();
}




