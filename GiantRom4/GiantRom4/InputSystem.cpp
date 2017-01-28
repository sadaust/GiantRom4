#include "InputSystem.h"
#include "Engine.h"

InputSystem::InputSystem() {
	_triggerThreshold = 128;
	_stickThreshold = 32767 / 2;
	repeatTime = 0.1f;
}

InputSystem::~InputSystem() {

}

void InputSystem::shutdown() {
	if(m_pDIKeyboard) {
		m_pDIKeyboard->Release();
		m_pDIKeyboard = 0;
	}

	if(m_pDIMouse) {
		m_pDIMouse->Release();
		m_pDIMouse = 0;
	}

	if(m_pDIObj) {
		m_pDIObj->Release();
		m_pDIObj = 0;
	}

	XInputEnable(false);
}

//create button bind
void InputSystem::bind(inputList input, LPCSTR name) {
	button temp;
	temp.name = name;
	temp._tRepeat = 0;
	temp.flags = 0x00;
	temp.buttons[0] = NO_INPUT;
	temp.buttons[1] = NO_INPUT;
	temp.buttons[2] = NO_INPUT;
	for(int i = 0; i < boundButton.size(); ++i) {
		if(boundButton[i].name == temp.name) {
			for(int z = 0; z < 3;++z) {
				if(boundButton[i].buttons[z] == NO_INPUT) {
					boundButton[i].buttons[z] = input;
					return;
				}
			}
			return;
		}
	}
	temp.buttons[0] = input;
	boundButton.push_back(temp);
}

void InputSystem::clearBinds() {
	while(boundAxis.size() > 0)
		boundAxis.pop_back();
	while(boundButton.size() > 0)
		boundButton.pop_back();
}

//create axis bind
void InputSystem::bind(inputList positive, inputList negitive, LPCSTR name) {
	axis temp;
	temp.name = name;
	for(int i = 0; i < boundAxis.size(); ++i) {
		if(boundAxis[i].name == temp.name) {
			return;
		}
	}
	temp.positive = positive;
	temp.negitive = negitive;
	boundAxis.push_back(temp);
}

float InputSystem::getBind(LPCSTR Name) {
	for(int i = 0; i < boundAxis.size(); ++i) {
		if(boundAxis[i].name == Name) {
			return getAxis(boundAxis[i].positive) - getAxis(boundAxis[i].negitive);
		}
	}

	for(int i = 0; i < boundButton.size(); ++i) {
		if(boundButton[i].name == Name) {
			for(int z = 0; z < 3; ++z) {
				if(getButton(boundButton[i].buttons[z]))
					return 1;
			}
			return 0;
		}
	}

	return 0;
}

float InputSystem::getAxis(LPCSTR Name) {
	for(int i = 0; i < boundAxis.size(); ++i) {
		if(boundAxis[i].name == Name) {
			return getAxis(boundAxis[i].positive) - getAxis(boundAxis[i].negitive);
		}
	}
	return 0;
}

bool InputSystem::getButton(LPCSTR Name) {
	for(int i = 0; i < boundButton.size(); ++i) {
		if(boundButton[i].name == Name) {
			for(int z = 0; z < 3; ++z) {
				if(getButton(boundButton[i].buttons[z]))
					return true;
			}
			return false;
		}
	}
	return false;
}

void InputSystem::init(HWND& hWnd, HINSTANCE& hInst) {
	DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&m_pDIObj,NULL);

	m_pDIObj->CreateDevice(GUID_SysKeyboard,&m_pDIKeyboard,NULL);
	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard); 
	m_pDIKeyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	m_pDIObj->CreateDevice(GUID_SysMouse,&m_pDIMouse,NULL);
	m_pDIMouse->SetDataFormat(&c_dfDIMouse2);
	m_pDIMouse->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	XInputEnable(true);
}

void InputSystem::update() {
	ZeroMemory(_keyboard,sizeof(_keyboard));
	ZeroMemory(&_mouse,sizeof(_mouse));
	if(m_pDIKeyboard->Acquire() != DIERR_INPUTLOST)
		m_pDIKeyboard->GetDeviceState(sizeof(_keyboard),(LPVOID)&_keyboard);
	if(m_pDIMouse->Acquire()!= DIERR_INPUTLOST)
		m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2),&_mouse);
	for(int i = 0; i < 4; ++i) {
		XInputGetState(i,&_pads[i]);
	}
	//update button flags
	char tempFlag;
	for(int i = 0; i < boundButton.size(); ++i) {
		tempFlag = 0x00;
		if(getBind(boundButton[i].name.c_str())) {
			if(boundButton[i].flags & buttonFlags::_held) {
				tempFlag = buttonFlags::_held;
				boundButton[i]._tRepeat -= Engine::instance()->dt();
				if(boundButton[i]._tRepeat <= 0) {
					tempFlag = tempFlag | buttonFlags::_repeat;
					boundButton[i]._tRepeat = repeatTime;
				}
			} else {
				tempFlag = buttonFlags::_pushed;
				tempFlag = tempFlag | buttonFlags::_held;
				tempFlag = tempFlag | buttonFlags::_repeat;
				boundButton[i]._tRepeat = repeatTime;
			}
		} else {
			if(boundButton[i].flags & buttonFlags::_held) {
				tempFlag = buttonFlags::_released;
			}
		}
		boundButton[i].flags = tempFlag;
	}
}

char InputSystem::getFlag(LPCSTR Name) {
	for(int i = 0; i < boundButton.size(); ++i) {
		if(boundButton[i].name == Name) {
			return boundButton[i].flags;
		}
	}
	return 0x00;
}

void InputSystem::restart(HWND& hWnd, HINSTANCE& hInst) {
	shutdown();
	init(hWnd,hInst);
}


float InputSystem::getAxis(inputList input) {
	switch(input) {
		//pad 1
	case pad1_A:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad1_B:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad1_X:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad1_Y:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad1_RB:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad1_LB:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad1_LT:
		return ((float)_pads[0].Gamepad.bLeftTrigger) / 255.0f;
	case pad1_RT:
		return ((float)_pads[0].Gamepad.bRightTrigger) / 255.0f;
	case pad1_UP:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad1_RIGHT:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad1_LEFT:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad1_DOWN:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad1_START:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad1_BACK:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad1_LS:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad1_RS:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad1_LXp:
		if(_pads[0].Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[0].Gamepad.sThumbLX)) / 32767.0f;
		return 0;
		//left x negative
	case pad1_LXn:
		if(_pads[0].Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			return ((float)(_pads[0].Gamepad.sThumbLX)) / -32767.0f;
		}
		return 0;
		//left y positive
	case pad1_LYp:
		if(_pads[0].Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[0].Gamepad.sThumbLY)) / 32767.0f;
		return 0;
		//left y negative
	case pad1_LYn:
		if(_pads[0].Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[0].Gamepad.sThumbLY)) / -32767.0f;
		return 0;
		//right x positive
	case pad1_RXp:
		if(_pads[0].Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[0].Gamepad.sThumbRX)) / 32767.0f;
		return 0;
		//right x negative
	case pad1_RXn:
		if(_pads[0].Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[0].Gamepad.sThumbRX)) / -32767.0f;
		return 0;
		//right y positive
	case pad1_RYp:
		if(_pads[0].Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[0].Gamepad.sThumbRY)) / 32767.0f;
		return 0;
		//right y negative
	case pad1_RYn:
		if(_pads[0].Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[0].Gamepad.sThumbRY)) / -32767.0f;
		return 0;
		//end pad 1

		//pad 2
	case pad2_A:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad2_B:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad2_X:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad2_Y:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad2_RB:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad2_LB:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad2_LT:
		return ((float)_pads[1].Gamepad.bLeftTrigger) / 255.0f;
	case pad2_RT:
		return ((float)_pads[1].Gamepad.bRightTrigger) / 255.0f;
	case pad2_UP:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad2_RIGHT:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad2_LEFT:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad2_DOWN:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad2_START:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad2_BACK:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad2_LS:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad2_RS:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad2_LXp:
		if(_pads[1].Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[1].Gamepad.sThumbLX)) / 32767.0f;
		return 0;
		//left x negative
	case pad2_LXn:
		if(_pads[1].Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			return ((float)(_pads[1].Gamepad.sThumbLX)) / -32767.0f;
		}
		return 0;
		//left y positive
	case pad2_LYp:
		if(_pads[1].Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[1].Gamepad.sThumbLY)) / 32767.0f;
		return 0;
		//left y negative
	case pad2_LYn:
		if(_pads[1].Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[1].Gamepad.sThumbLY)) / -32767.0f;
		return 0;
		//right x positive
	case pad2_RXp:
		if(_pads[1].Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[1].Gamepad.sThumbRX)) / 32767.0f;
		return 0;
		//right x negative
	case pad2_RXn:
		if(_pads[1].Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[1].Gamepad.sThumbRX)) / -32767.0f;
		return 0;
		//right y positive
	case pad2_RYp:
		if(_pads[1].Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[1].Gamepad.sThumbRY)) / 32767.0f;
		return 0;
		//right y negative
	case pad2_RYn:
		if(_pads[1].Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[1].Gamepad.sThumbRY)) / -32767.0f;
		return 0;
		//end pad 2

		//pad 3
	case pad3_A:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad3_B:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad3_X:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad3_Y:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad3_RB:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad3_LB:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad3_LT:
		return ((float)_pads[2].Gamepad.bLeftTrigger) / 255.0f;
	case pad3_RT:
		return ((float)_pads[2].Gamepad.bRightTrigger) / 255.0f;
	case pad3_UP:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad3_RIGHT:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad3_LEFT:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad3_DOWN:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad3_START:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad3_BACK:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad3_LS:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad3_RS:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad3_LXp:
		if(_pads[2].Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[2].Gamepad.sThumbLX)) / 32767.0f;
		return 0;
		//left x negative
	case pad3_LXn:
		if(_pads[2].Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			return ((float)(_pads[2].Gamepad.sThumbLX)) / -32767.0f;
		}
		return 0;
		//left y positive
	case pad3_LYp:
		if(_pads[2].Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[2].Gamepad.sThumbLY)) / 32767.0f;
		return 0;
		//left y negative
	case pad3_LYn:
		if(_pads[2].Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[2].Gamepad.sThumbLY)) / -32767.0f;
		return 0;
		//right x positive
	case pad3_RXp:
		if(_pads[2].Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[2].Gamepad.sThumbRX)) / 32767.0f;
		return 0;
		//right x negative
	case pad3_RXn:
		if(_pads[2].Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[2].Gamepad.sThumbRX)) / -32767.0f;
		return 0;
		//right y positive
	case pad3_RYp:
		if(_pads[2].Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[2].Gamepad.sThumbRY)) / 32767.0f;
		return 0;
		//right y negative
	case pad3_RYn:
		if(_pads[2].Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[2].Gamepad.sThumbRY)) / -32767.0f;
		return 0;
		//end pad 3

		//pad 4
	case pad4_A:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad4_B:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad4_X:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad4_Y:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad4_RB:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad4_LB:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad4_LT:
		return ((float)_pads[3].Gamepad.bLeftTrigger) / 255.0f;
	case pad4_RT:
		return ((float)_pads[3].Gamepad.bRightTrigger) / 255.0f;
	case pad4_UP:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad4_RIGHT:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad4_LEFT:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad4_DOWN:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad4_START:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad4_BACK:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad4_LS:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad4_RS:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad4_LXp:
		if(_pads[3].Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[3].Gamepad.sThumbLX)) / 32767.0f;
		return 0;
		//left x negative
	case pad4_LXn:
		if(_pads[3].Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
			return ((float)(_pads[3].Gamepad.sThumbLX)) / -32767.0f;
		}
		return 0;
		//left y positive
	case pad4_LYp:
		if(_pads[3].Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[3].Gamepad.sThumbLY)) / 32767.0f;
		return 0;
		//left y negative
	case pad4_LYn:
		if(_pads[3].Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return ((float)(_pads[3].Gamepad.sThumbLY)) / -32767.0f;
		return 0;
		//right x positive
	case pad4_RXp:
		if(_pads[3].Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[3].Gamepad.sThumbRX)) / 32767.0f;
		return 0;
		//right x negative
	case pad4_RXn:
		if(_pads[3].Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[3].Gamepad.sThumbRX)) / -32767.0f;
		return 0;
		//right y positive
	case pad4_RYp:
		if(_pads[3].Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[3].Gamepad.sThumbRY)) / 32767.0f;
		return 0;
		//right y negative
	case pad4_RYn:
		if(_pads[3].Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return ((float)(_pads[3].Gamepad.sThumbRY)) / -32767.0f;
		return 0;
		//end pad 4

		//mouse
	case mouse_0:
		return (bool)(_mouse.rgbButtons[0]&0x80);
	case mouse_1:
		return (bool)(_mouse.rgbButtons[1]&0x80);
	case mouse_2:
		return (bool)(_mouse.rgbButtons[2]&0x80);
	case mouse_3:
		return (bool)(_mouse.rgbButtons[3]&0x80);
	case mouse_4:
		return (bool)(_mouse.rgbButtons[4]&0x80);
	case mouse_5:
		return (bool)(_mouse.rgbButtons[5]&0x80);
	case mouse_6:
		return (bool)(_mouse.rgbButtons[6]&0x80);
	case mouse_7:
		return (bool)(_mouse.rgbButtons[7]&0x80);
	case mouse_WU:
		if(_mouse.lZ > 0) {
			return _mouse.lZ;
		}
		return 0;
	case mouse_WD:
		if(_mouse.lZ < 0)
			return -_mouse.lZ;
		return 0;
	case mouse_Xp:
		if(_mouse.lX > 0)
			return _mouse.lX;
		return 0;
	case mouse_Xn:
		if(_mouse.lX < 0)
			return -_mouse.lX;
		return 0;
	case mouse_Yp:
		if(_mouse.lY > 0)
			return _mouse.lY;
		return 0;
	case mouse_Yn:
		if(_mouse.lY < 0)
			return -_mouse.lY;
		return 0;
		//end mouse

		//keyboard
		//numpad
	case NP_0:
		return (bool)(_keyboard[DIK_NUMPAD0] & 0x80);
	case NP_1:
		return (bool)(_keyboard[DIK_NUMPAD1] & 0x80);
	case NP_2:
		return (bool)(_keyboard[DIK_NUMPAD2] & 0x80);
	case NP_3:
		return (bool)(_keyboard[DIK_NUMPAD3] & 0x80);
	case NP_4:
		return (bool)(_keyboard[DIK_NUMPAD4] & 0x80);
	case NP_5:
		return (bool)(_keyboard[DIK_NUMPAD5] & 0x80);
	case NP_6:
		return (bool)(_keyboard[DIK_NUMPAD6] & 0x80);
	case NP_7:
		return (bool)(_keyboard[DIK_NUMPAD7] & 0x80);
	case NP_8:
		return (bool)(_keyboard[DIK_NUMPAD8] & 0x80);
	case NP_9:
		return (bool)(_keyboard[DIK_NUMPAD9] & 0x80);
		//NP_.
	case NP_del:
		return (bool)(_keyboard[DIK_NUMPADPERIOD] & 0x80);
		//NP_/
	case NP_div:
		return (bool)(_keyboard[DIK_NUMPADSLASH] & 0x80);
		//NP_*
	case NP_mul:
		return (bool)(_keyboard[DIK_NUMPADSTAR] & 0x80);
		//NP_-
	case NP_sub:
		return (bool)(_keyboard[DIK_NUMPADMINUS] & 0x80);
		//NP_+
	case NP_add:
		return (bool)(_keyboard[DIK_NUMPADPLUS] & 0x80);
	case NP_ent:
		return (bool)(_keyboard[DIK_NUMPADENTER] & 0x80);
	case ESC:
		return (bool)(_keyboard[DIK_ESCAPE] & 0x80);
	case F1:
		return (bool)(_keyboard[DIK_F1] & 0x80);
	case F2:
		return (bool)(_keyboard[DIK_F2] & 0x80);
	case F3:
		return (bool)(_keyboard[DIK_F3] & 0x80);
	case F4:
		return (bool)(_keyboard[DIK_F4] & 0x80);
	case F5:
		return (bool)(_keyboard[DIK_F5] & 0x80);
	case F6:
		return (bool)(_keyboard[DIK_F6] & 0x80);
	case F7:
		return (bool)(_keyboard[DIK_F7] & 0x80);
	case F8:
		return (bool)(_keyboard[DIK_F8] & 0x80);
	case F9:
		return (bool)(_keyboard[DIK_F9] & 0x80);
	case F10:
		return (bool)(_keyboard[DIK_F10] & 0x80);
	case F11:
		return (bool)(_keyboard[DIK_F11] & 0x80);
	case F12:
		return (bool)(_keyboard[DIK_F12] & 0x80);
	case F13:
		return (bool)(_keyboard[DIK_F13] & 0x80);
	case F14:
		return (bool)(_keyboard[DIK_F14] & 0x80);
	case F15:
		return (bool)(_keyboard[DIK_F15] & 0x80);
		//~
	case TILD:
		return (bool)(_keyboard[DIK_GRAVE] & 0x80);
	case _1:
		return (bool)(_keyboard[DIK_1] & 0x80);
	case _2:
		return (bool)(_keyboard[DIK_2] & 0x80);
	case _3:
		return (bool)(_keyboard[DIK_3] & 0x80);
	case _4:
		return (bool)(_keyboard[DIK_4] & 0x80);
	case _5:
		return (bool)(_keyboard[DIK_5] & 0x80);
	case _6:
		return (bool)(_keyboard[DIK_6] & 0x80);
	case _7:
		return (bool)(_keyboard[DIK_7] & 0x80);
	case _8:
		return (bool)(_keyboard[DIK_8] & 0x80);
	case _9:
		return (bool)(_keyboard[DIK_9] & 0x80);
	case _0:
		return (bool)(_keyboard[DIK_0] & 0x80);
		//-
	case MINUS:
		return (bool)(_keyboard[DIK_MINUS] & 0x80);
		//=
	case EQUALS:
		return (bool)(_keyboard[DIK_EQUALS] & 0x80);
	case BACK:
		return (bool)(_keyboard[DIK_BACKSPACE] & 0x80);
	case TAB:
		return (bool)(_keyboard[DIK_TAB] & 0x80);
	case Q:
		return (bool)(_keyboard[DIK_Q] & 0x80);
	case W:
		return (bool)(_keyboard[DIK_W] & 0x80);
	case E:
		return (bool)(_keyboard[DIK_E] & 0x80);
	case R:
		return (bool)(_keyboard[DIK_R] & 0x80);
	case T:
		return (bool)(_keyboard[DIK_T] & 0x80);
	case Y:
		return (bool)(_keyboard[DIK_Y] & 0x80);
	case U:
		return (bool)(_keyboard[DIK_U] & 0x80);
	case I:
		return (bool)(_keyboard[DIK_I] & 0x80);
	case O:
		return (bool)(_keyboard[DIK_O] & 0x80);
	case P:
		return (bool)(_keyboard[DIK_P] & 0x80);
	case LBRACKET:
		return (bool)(_keyboard[DIK_LBRACKET] & 0x80);
	case RBRACKET:
		return (bool)(_keyboard[DIK_RBRACKET] & 0x80);
	case BACKSLASH:
		return (bool)(_keyboard[DIK_BACKSLASH] & 0x80);
	case CAPSLOCK:
		return (bool)(_keyboard[DIK_CAPSLOCK] & 0x80);
	case A:
		return (bool)(_keyboard[DIK_A] & 0x80);
	case S:
		return (bool)(_keyboard[DIK_S] & 0x80);
	case D:
		return (bool)(_keyboard[DIK_D] & 0x80);
	case F:
		return (bool)(_keyboard[DIK_F] & 0x80);
	case G:
		return (bool)(_keyboard[DIK_G] & 0x80);
	case H:
		return (bool)(_keyboard[DIK_H] & 0x80);
	case J:
		return (bool)(_keyboard[DIK_J] & 0x80);
	case K:
		return (bool)(_keyboard[DIK_K] & 0x80);
	case L:
		return (bool)(_keyboard[DIK_L] & 0x80);
	case SEMICOLON:
		return (bool)(_keyboard[DIK_SEMICOLON] & 0x80);
	case APOSTROPHE:
		return (bool)(_keyboard[DIK_APOSTROPHE] & 0x80);
	case RETURN:
		return (bool)(_keyboard[DIK_RETURN] & 0x80);
	case LSHIFT:
		return (bool)(_keyboard[DIK_LSHIFT] & 0x80);
	case Z:
		return (bool)(_keyboard[DIK_Z] & 0x80);
	case X:
		return (bool)(_keyboard[DIK_X] & 0x80);
	case C:
		return (bool)(_keyboard[DIK_C] & 0x80);
	case V:
		return (bool)(_keyboard[DIK_V] & 0x80);
	case B:
		return (bool)(_keyboard[DIK_B] & 0x80);
	case N:
		return (bool)(_keyboard[DIK_N] & 0x80);
	case M:
		return (bool)(_keyboard[DIK_M] & 0x80);
	case COMMA:
		return (bool)(_keyboard[DIK_COMMA] & 0x80);
	case PERIOD:
		return (bool)(_keyboard[DIK_PERIOD] & 0x80);
	case SLASH:
		return (bool)(_keyboard[DIK_SLASH] & 0x80);
	case RSHIFT:
		return (bool)(_keyboard[DIK_RSHIFT] & 0x80);
	case LCTRL:
		return (bool)(_keyboard[DIK_LCONTROL] & 0x80);
	case LALT:
		return (bool)(_keyboard[DIK_LALT] & 0x80);
	case SPACE:
		return (bool)(_keyboard[DIK_SPACE] & 0x80);
	case RALT:
		return (bool)(_keyboard[DIK_RALT] & 0x80);
	case RCTRL:
		return (bool)(_keyboard[DIK_RCONTROL] & 0x80);
	case INS:
		return (bool)(_keyboard[DIK_INSERT] & 0x80);
	case HOME:
		return (bool)(_keyboard[DIK_HOME] & 0x80);
	case PAGEUP:
		return (bool)(_keyboard[DIK_PGUP] & 0x80);
	case DEL:
		return (bool)(_keyboard[DIK_DELETE] & 0x80);
	case END:
		return (bool)(_keyboard[DIK_END] & 0x80);
	case PAGEDOWN:
		return (bool)(_keyboard[DIK_PGDN] & 0x80);
	case UP:
		return (bool)(_keyboard[DIK_UP] & 0x80);
	case LEFT:
		return (bool)(_keyboard[DIK_LEFT] & 0x80);
	case DOWN:
		return (bool)(_keyboard[DIK_DOWN] & 0x80);
	case RIGHT:
		return (bool)(_keyboard[DIK_RIGHT] & 0x80);
		//end keyboard
	default:
		return 0;
	}
	return 0;
}

bool InputSystem::getButton(inputList input) {
	switch(input) {
		//pad 1
	case pad1_A:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad1_B:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad1_X:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad1_Y:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad1_RB:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad1_LB:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad1_LT:
		return _pads[0].Gamepad.bLeftTrigger >= _triggerThreshold;
	case pad1_RT:
		return _pads[0].Gamepad.bRightTrigger >= _triggerThreshold;
	case pad1_UP:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad1_RIGHT:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad1_LEFT:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad1_DOWN:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad1_START:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad1_BACK:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad1_LS:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad1_RS:
		return (bool)(_pads[0].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad1_LXp:
		return _pads[0].Gamepad.sThumbLX >= _stickThreshold;
		//left x negative
	case pad1_LXn:
		return _pads[0].Gamepad.sThumbLX <= -_stickThreshold;
		//left y positive
	case pad1_LYp:
		return _pads[0].Gamepad.sThumbLY >= _stickThreshold;;
		//left y negative
	case pad1_LYn:
		return _pads[0].Gamepad.sThumbLY <= -_stickThreshold;
		//right x positive
	case pad1_RXp:
		return _pads[0].Gamepad.sThumbRX >= _stickThreshold;
		//right x negative
	case pad1_RXn:
		return _pads[0].Gamepad.sThumbRX <= -_stickThreshold;
		//right y positive
	case pad1_RYp:
		return _pads[0].Gamepad.sThumbRY >= _stickThreshold;
		//right y negative
	case pad1_RYn:
		return _pads[0].Gamepad.sThumbRY <= -_stickThreshold;
		//end pad 1

		//pad 2
	case pad2_A:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad2_B:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad2_X:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad2_Y:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad2_RB:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad2_LB:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad2_LT:
		return _pads[1].Gamepad.bLeftTrigger >= _triggerThreshold;
	case pad2_RT:
		return _pads[1].Gamepad.bRightTrigger >= _triggerThreshold;
	case pad2_UP:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad2_RIGHT:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad2_LEFT:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad2_DOWN:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad2_START:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad2_BACK:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad2_LS:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad2_RS:
		return (bool)(_pads[1].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad2_LXp:
		return _pads[1].Gamepad.sThumbLX >= _stickThreshold;
		//left x negative
	case pad2_LXn:
		return _pads[1].Gamepad.sThumbLX <= -_stickThreshold;
		//left y positive
	case pad2_LYp:
		return _pads[1].Gamepad.sThumbLY >= _stickThreshold;;
		//left y negative
	case pad2_LYn:
		return _pads[1].Gamepad.sThumbLY <= -_stickThreshold;
		//right x positive
	case pad2_RXp:
		return _pads[1].Gamepad.sThumbRX >= _stickThreshold;
		//right x negative
	case pad2_RXn:
		return _pads[1].Gamepad.sThumbRX <= -_stickThreshold;
		//right y positive
	case pad2_RYp:
		return _pads[1].Gamepad.sThumbRY >= _stickThreshold;
		//right y negative
	case pad2_RYn:
		return _pads[1].Gamepad.sThumbRY <= -_stickThreshold;
		//end pad 2

		//pad 3
	case pad3_A:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad3_B:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad3_X:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad3_Y:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad3_RB:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad3_LB:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad3_LT:
		return _pads[2].Gamepad.bLeftTrigger >= _triggerThreshold;
	case pad3_RT:
		return _pads[2].Gamepad.bRightTrigger >= _triggerThreshold;
	case pad3_UP:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad3_RIGHT:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad3_LEFT:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad3_DOWN:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad3_START:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad3_BACK:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad3_LS:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad3_RS:
		return (bool)(_pads[2].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad3_LXp:
		return _pads[2].Gamepad.sThumbLX >= _stickThreshold;
		//left x negative
	case pad3_LXn:
		return _pads[2].Gamepad.sThumbLX <= -_stickThreshold;
		//left y positive
	case pad3_LYp:
		return _pads[2].Gamepad.sThumbLY >= _stickThreshold;;
		//left y negative
	case pad3_LYn:
		return _pads[2].Gamepad.sThumbLY <= -_stickThreshold;
		//right x positive
	case pad3_RXp:
		return _pads[2].Gamepad.sThumbRX >= _stickThreshold;
		//right x negative
	case pad3_RXn:
		return _pads[2].Gamepad.sThumbRX <= -_stickThreshold;
		//right y positive
	case pad3_RYp:
		return _pads[2].Gamepad.sThumbRY >= _stickThreshold;
		//right y negative
	case pad3_RYn:
		return _pads[2].Gamepad.sThumbRY <= -_stickThreshold;
		//end pad 3
		
		//pad 4
	case pad4_A:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_A);
	case pad4_B:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_B);
	case pad4_X:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_X);
	case pad4_Y:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_Y);
	case pad4_RB:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER);
	case pad4_LB:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_SHOULDER);
	case pad4_LT:
		return _pads[3].Gamepad.bLeftTrigger >= _triggerThreshold;
	case pad4_RT:
		return _pads[3].Gamepad.bRightTrigger >= _triggerThreshold;
	case pad4_UP:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_UP);
	case pad4_RIGHT:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_RIGHT);
	case pad4_LEFT:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_LEFT);
	case pad4_DOWN:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_DPAD_DOWN);
	case pad4_START:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_START);
	case pad4_BACK:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_BACK);
		//click left stick
	case pad4_LS:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_LEFT_THUMB);
		//click right stick
	case pad4_RS:
		return (bool)(_pads[3].Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_THUMB);
		//left x positive
	case pad4_LXp:
		return _pads[3].Gamepad.sThumbLX >= _stickThreshold;
		//left x negative
	case pad4_LXn:
		return _pads[3].Gamepad.sThumbLX <= -_stickThreshold;
		//left y positive
	case pad4_LYp:
		return _pads[3].Gamepad.sThumbLY >= _stickThreshold;;
		//left y negative
	case pad4_LYn:
		return _pads[3].Gamepad.sThumbLY <= -_stickThreshold;
		//right x positive
	case pad4_RXp:
		return _pads[3].Gamepad.sThumbRX >= _stickThreshold;
		//right x negative
	case pad4_RXn:
		return _pads[3].Gamepad.sThumbRX <= -_stickThreshold;
		//right y positive
	case pad4_RYp:
		return _pads[3].Gamepad.sThumbRY >= _stickThreshold;
		//right y negative
	case pad4_RYn:
		return _pads[3].Gamepad.sThumbRY <= -_stickThreshold;
		//end pad 4

		//mouse
	case mouse_0:
		return (bool)(_mouse.rgbButtons[0]&0x80);
	case mouse_1:
		return (bool)(_mouse.rgbButtons[1]&0x80);
	case mouse_2:
		return (bool)(_mouse.rgbButtons[2]&0x80);
	case mouse_3:
		return (bool)(_mouse.rgbButtons[3]&0x80);
	case mouse_4:
		return (bool)(_mouse.rgbButtons[4]&0x80);
	case mouse_5:
		return (bool)(_mouse.rgbButtons[5]&0x80);
	case mouse_6:
		return (bool)(_mouse.rgbButtons[6]&0x80);
	case mouse_7:
		return (bool)(_mouse.rgbButtons[7]&0x80);
	case mouse_WU:
		if(_mouse.lZ > 0) {
			return (bool)_mouse.lZ;
		}
		return 0;
	case mouse_WD:
		if(_mouse.lZ < 0)
			return (bool)_mouse.lZ;
		return 0;
	case mouse_Xp:
		if(_mouse.lX > 0)
			return (bool)_mouse.lX;
		return 0;
	case mouse_Xn:
		if(_mouse.lX < 0)
			return (bool)_mouse.lX;
		return 0;
	case mouse_Yp:
		if(_mouse.lY > 0)
			return (bool)_mouse.lY;
		return 0;
	case mouse_Yn:
		if(_mouse.lY < 0)
			return (bool)_mouse.lY;
		return 0;
		//end mouse

		//keyboard
		//numpad
	case NP_0:
		return (bool)(_keyboard[DIK_NUMPAD0] & 0x80);
	case NP_1:
		return (bool)(_keyboard[DIK_NUMPAD1] & 0x80);
	case NP_2:
		return (bool)(_keyboard[DIK_NUMPAD2] & 0x80);
	case NP_3:
		return (bool)(_keyboard[DIK_NUMPAD3] & 0x80);
	case NP_4:
		return (bool)(_keyboard[DIK_NUMPAD4] & 0x80);
	case NP_5:
		return (bool)(_keyboard[DIK_NUMPAD5] & 0x80);
	case NP_6:
		return (bool)(_keyboard[DIK_NUMPAD6] & 0x80);
	case NP_7:
		return (bool)(_keyboard[DIK_NUMPAD7] & 0x80);
	case NP_8:
		return (bool)(_keyboard[DIK_NUMPAD8] & 0x80);
	case NP_9:
		return (bool)(_keyboard[DIK_NUMPAD9] & 0x80);
		//NP_.
	case NP_del:
		return (bool)(_keyboard[DIK_NUMPADPERIOD] & 0x80);
		//NP_/
	case NP_div:
		return (bool)(_keyboard[DIK_NUMPADSLASH] & 0x80);
		//NP_*
	case NP_mul:
		return (bool)(_keyboard[DIK_NUMPADSTAR] & 0x80);
		//NP_-
	case NP_sub:
		return (bool)(_keyboard[DIK_NUMPADMINUS] & 0x80);
		//NP_+
	case NP_add:
		return (bool)(_keyboard[DIK_NUMPADPLUS] & 0x80);
	case NP_ent:
		return (bool)(_keyboard[DIK_NUMPADENTER] & 0x80);
	case ESC:
		return (bool)(_keyboard[DIK_ESCAPE] & 0x80);
	case F1:
		return (bool)(_keyboard[DIK_F1] & 0x80);
	case F2:
		return (bool)(_keyboard[DIK_F2] & 0x80);
	case F3:
		return (bool)(_keyboard[DIK_F3] & 0x80);
	case F4:
		return (bool)(_keyboard[DIK_F4] & 0x80);
	case F5:
		return (bool)(_keyboard[DIK_F5] & 0x80);
	case F6:
		return (bool)(_keyboard[DIK_F6] & 0x80);
	case F7:
		return (bool)(_keyboard[DIK_F7] & 0x80);
	case F8:
		return (bool)(_keyboard[DIK_F8] & 0x80);
	case F9:
		return (bool)(_keyboard[DIK_F9] & 0x80);
	case F10:
		return (bool)(_keyboard[DIK_F10] & 0x80);
	case F11:
		return (bool)(_keyboard[DIK_F11] & 0x80);
	case F12:
		return (bool)(_keyboard[DIK_F12] & 0x80);
	case F13:
		return (bool)(_keyboard[DIK_F13] & 0x80);
	case F14:
		return (bool)(_keyboard[DIK_F14] & 0x80);
	case F15:
		return (bool)(_keyboard[DIK_F15] & 0x80);
		//~
	case TILD:
		return (bool)(_keyboard[DIK_GRAVE] & 0x80);
	case _1:
		return (bool)(_keyboard[DIK_1] & 0x80);
	case _2:
		return (bool)(_keyboard[DIK_2] & 0x80);
	case _3:
		return (bool)(_keyboard[DIK_3] & 0x80);
	case _4:
		return (bool)(_keyboard[DIK_4] & 0x80);
	case _5:
		return (bool)(_keyboard[DIK_5] & 0x80);
	case _6:
		return (bool)(_keyboard[DIK_6] & 0x80);
	case _7:
		return (bool)(_keyboard[DIK_7] & 0x80);
	case _8:
		return (bool)(_keyboard[DIK_8] & 0x80);
	case _9:
		return (bool)(_keyboard[DIK_9] & 0x80);
	case _0:
		return (bool)(_keyboard[DIK_0] & 0x80);
		//-
	case MINUS:
		return (bool)(_keyboard[DIK_MINUS] & 0x80);
		//=
	case EQUALS:
		return (bool)(_keyboard[DIK_EQUALS] & 0x80);
	case BACK:
		return (bool)(_keyboard[DIK_BACKSPACE] & 0x80);
	case TAB:
		return (bool)(_keyboard[DIK_TAB] & 0x80);
	case Q:
		return (bool)(_keyboard[DIK_Q] & 0x80);
	case W:
		return (bool)(_keyboard[DIK_W] & 0x80);
	case E:
		return (bool)(_keyboard[DIK_E] & 0x80);
	case R:
		return (bool)(_keyboard[DIK_R] & 0x80);
	case T:
		return (bool)(_keyboard[DIK_T] & 0x80);
	case Y:
		return (bool)(_keyboard[DIK_Y] & 0x80);
	case U:
		return (bool)(_keyboard[DIK_U] & 0x80);
	case I:
		return (bool)(_keyboard[DIK_I] & 0x80);
	case O:
		return (bool)(_keyboard[DIK_O] & 0x80);
	case P:
		return (bool)(_keyboard[DIK_P] & 0x80);
	case LBRACKET:
		return (bool)(_keyboard[DIK_LBRACKET] & 0x80);
	case RBRACKET:
		return (bool)(_keyboard[DIK_RBRACKET] & 0x80);
	case BACKSLASH:
		return (bool)(_keyboard[DIK_BACKSLASH] & 0x80);
	case CAPSLOCK:
		return (bool)(_keyboard[DIK_CAPSLOCK] & 0x80);
	case A:
		return (bool)(_keyboard[DIK_A] & 0x80);
	case S:
		return (bool)(_keyboard[DIK_S] & 0x80);
	case D:
		return (bool)(_keyboard[DIK_D] & 0x80);
	case F:
		return (bool)(_keyboard[DIK_F] & 0x80);
	case G:
		return (bool)(_keyboard[DIK_G] & 0x80);
	case H:
		return (bool)(_keyboard[DIK_H] & 0x80);
	case J:
		return (bool)(_keyboard[DIK_J] & 0x80);
	case K:
		return (bool)(_keyboard[DIK_K] & 0x80);
	case L:
		return (bool)(_keyboard[DIK_L] & 0x80);
	case SEMICOLON:
		return (bool)(_keyboard[DIK_SEMICOLON] & 0x80);
	case APOSTROPHE:
		return (bool)(_keyboard[DIK_APOSTROPHE] & 0x80);
	case RETURN:
		return (bool)(_keyboard[DIK_RETURN] & 0x80);
	case LSHIFT:
		return (bool)(_keyboard[DIK_LSHIFT] & 0x80);
	case Z:
		return (bool)(_keyboard[DIK_Z] & 0x80);
	case X:
		return (bool)(_keyboard[DIK_X] & 0x80);
	case C:
		return (bool)(_keyboard[DIK_C] & 0x80);
	case V:
		return (bool)(_keyboard[DIK_V] & 0x80);
	case B:
		return (bool)(_keyboard[DIK_B] & 0x80);
	case N:
		return (bool)(_keyboard[DIK_N] & 0x80);
	case M:
		return (bool)(_keyboard[DIK_M] & 0x80);
	case COMMA:
		return (bool)(_keyboard[DIK_COMMA] & 0x80);
	case PERIOD:
		return (bool)(_keyboard[DIK_PERIOD] & 0x80);
	case SLASH:
		return (bool)(_keyboard[DIK_SLASH] & 0x80);
	case RSHIFT:
		return (bool)(_keyboard[DIK_RSHIFT] & 0x80);
	case LCTRL:
		return (bool)(_keyboard[DIK_LCONTROL] & 0x80);
	case LALT:
		return (bool)(_keyboard[DIK_LALT] & 0x80);
	case SPACE:
		return (bool)(_keyboard[DIK_SPACE] & 0x80);
	case RALT:
		return (bool)(_keyboard[DIK_RALT] & 0x80);
	case RCTRL:
		return (bool)(_keyboard[DIK_RCONTROL] & 0x80);
	case INS:
		return (bool)(_keyboard[DIK_INSERT] & 0x80);
	case HOME:
		return (bool)(_keyboard[DIK_HOME] & 0x80);
	case PAGEUP:
		return (bool)(_keyboard[DIK_PGUP] & 0x80);
	case DEL:
		return (bool)(_keyboard[DIK_DELETE] & 0x80);
	case END:
		return (bool)(_keyboard[DIK_END] & 0x80);
	case PAGEDOWN:
		return (bool)(_keyboard[DIK_PGDN] & 0x80);
	case UP:
		return (bool)(_keyboard[DIK_UP] & 0x80);
	case LEFT:
		return (bool)(_keyboard[DIK_LEFT] & 0x80);
	case DOWN:
		return (bool)(_keyboard[DIK_DOWN] & 0x80);
	case RIGHT:
		return (bool)(_keyboard[DIK_RIGHT] & 0x80);
		//end keyboard
	default:
		return 0;
	}
	return 0;
}
