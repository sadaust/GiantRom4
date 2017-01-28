/*=======================================
=InputSystem.h/.cpp						=
=by Tyler van Gastel					=
=latest update 11/30/2015				=
=user input using xinput/directinput	=
=========================================*/
#pragma once
#include <Windows.h>
#include <dinput.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <string>
#include <vector>

enum inputList {
	NO_INPUT,
	//pad 1
	pad1_A,
	pad1_B,
	pad1_X,
	pad1_Y,
	pad1_RB,
	pad1_LB,
	pad1_LT,
	pad1_RT,
	pad1_UP,
	pad1_RIGHT,
	pad1_LEFT,
	pad1_DOWN,
	pad1_START,
	pad1_BACK,
	//click left stick
	pad1_LS,
	//click right stick
	pad1_RS,
	//left x positive
	pad1_LXp,
	//left x negative
	pad1_LXn,
	//left y positive
	pad1_LYp,
	//left y negative
	pad1_LYn,
	//right x positive
	pad1_RXp,
	//right x negative
	pad1_RXn,
	//right y positive
	pad1_RYp,
	//right y negative
	pad1_RYn,

	//pad 2
	pad2_A,
	pad2_B,
	pad2_X,
	pad2_Y,
	pad2_RB,
	pad2_LB,
	pad2_LT,
	pad2_RT,
	pad2_UP,
	pad2_RIGHT,
	pad2_LEFT,
	pad2_DOWN,
	pad2_START,
	pad2_BACK,
	//click left stick
	pad2_LS,
	//click right stick
	pad2_RS,
	//left x positive
	pad2_LXp,
	//left x negative
	pad2_LXn,
	//left y positive
	pad2_LYp,
	//left y negative
	pad2_LYn,
	//right x positive
	pad2_RXp,
	//right x negative
	pad2_RXn,
	//right y positive
	pad2_RYp,
	//right y negative
	pad2_RYn,
	
	//pad 3
	pad3_A,
	pad3_B,
	pad3_X,
	pad3_Y,
	pad3_RB,
	pad3_LB,
	pad3_LT,
	pad3_RT,
	pad3_UP,
	pad3_RIGHT,
	pad3_LEFT,
	pad3_DOWN,
	pad3_START,
	pad3_BACK,
	//click left stick
	pad3_LS,
	//click right stick
	pad3_RS,
	//left x positive
	pad3_LXp,
	//left x negative
	pad3_LXn,
	//left y positive
	pad3_LYp,
	//left y negative
	pad3_LYn,
	//right x positive
	pad3_RXp,
	//right x negative
	pad3_RXn,
	//right y positive
	pad3_RYp,
	//right y negative
	pad3_RYn,

	//pad 4
	pad4_A,
	pad4_B,
	pad4_X,
	pad4_Y,
	pad4_RB,
	pad4_LB,
	pad4_LT,
	pad4_RT,
	pad4_UP,
	pad4_RIGHT,
	pad4_LEFT,
	pad4_DOWN,
	pad4_START,
	pad4_BACK,
	//click left stick
	pad4_LS,
	//click right stick
	pad4_RS,
	//left x positive
	pad4_LXp,
	//left x negative
	pad4_LXn,
	//left y positive
	pad4_LYp,
	//left y negative
	pad4_LYn,
	//right x positive
	pad4_RXp,
	//right x negative
	pad4_RXn,
	//right y positive
	pad4_RYp,
	//right y negative
	pad4_RYn,

	//mouse
	mouse_0,
	mouse_1,
	mouse_2,
	mouse_3,
	mouse_4,
	mouse_5,
	mouse_6,
	mouse_7,
	mouse_WU,
	mouse_WD,
	mouse_Xp,
	mouse_Xn,
	mouse_Yp,
	mouse_Yn,

	//keyboard
	//numpad
	NP_0,
	NP_1,
	NP_2,
	NP_3,
	NP_4,
	NP_5,
	NP_6,
	NP_7,
	NP_8,
	NP_9,
	//NP_.
	NP_del,
	//NP_/
	NP_div,
	//NP_*
	NP_mul,
	//NP_-
	NP_sub,
	//NP_+
	NP_add,
	NP_ent,

	ESC,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	//~
	TILD,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	_0,
	//-
	MINUS,
	//=
	EQUALS,
	BACK,
	TAB,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	LBRACKET,
	RBRACKET,
	BACKSLASH,
	CAPSLOCK,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	SEMICOLON,
	APOSTROPHE,
	RETURN,
	LSHIFT,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	COMMA,
	PERIOD,
	SLASH,
	RSHIFT,
	LCTRL,
	LALT,
	SPACE,
	RALT,
	RCTRL,
	INS,
	HOME,
	PAGEUP,
	DEL,
	END,
	PAGEDOWN,
	UP,
	LEFT,
	DOWN,
	RIGHT
};

namespace buttonFlags {
static char _pushed = 0x01;
static char _held = 0x02;
static char _released = 0x04;
static char _repeat = 0x08;
}

struct axis {
	std::string name;
	inputList positive;
	inputList negitive;
};

struct button {
	std::string name;
	float _tRepeat;
	char flags;
	inputList buttons[3];
};

class InputSystem {
private:
	float repeatTime;
	//direct input interface
	IDirectInput8* m_pDIObj;
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
	//input buffer
	char _keyboard[256];
	DIMOUSESTATE2 _mouse;
	XINPUT_STATE _pads[4];
	BYTE _triggerThreshold;
	short _stickThreshold;
	std::vector<axis> boundAxis;
	std::vector<button> boundButton;
public:
	InputSystem();
	~InputSystem();
	void setRepeat(float repeat) { repeatTime = repeat;}
	void shutdown();
	void init(HWND& hWnd,HINSTANCE& hInst);
	void restart(HWND& hWnd,HINSTANCE& hInst);
	float getBind(LPCSTR Name);
	char getFlag(LPCSTR Name);
	//create button bind
	void bind(inputList input, LPCSTR name);
	//create axis bind
	void bind(inputList positive, inputList negitive, LPCSTR name);
	void clearBinds();
	/*
	returns input as a value between 0 and 1
	mouse returns the delta for that axis in 0+ format
	*/
	float getAxis(inputList input);
	float getAxis(LPCSTR Name);
	/*
	returns button state true for pushed false for not pushed
	mouse axis and sticks not recommended
	*/
	bool getButton(inputList input);
	bool getButton(LPCSTR Name);
	DIMOUSESTATE2* mouseBuffer() {return &_mouse;}
	char* keyboardBuffer() {return _keyboard;}
	XINPUT_STATE* padBuffer(int pad) {return &_pads[pad];}
	void update();
};