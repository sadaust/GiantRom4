/*===================================================
=Engine.h/.cpp										=
=by Tyler van Gastel								=
=latest update 08/05/2015							=
=Holds graphics, sound, input and resources manager	=
=====================================================*/

#pragma once
#include "Graphics.h"
#include "ResourceManager.h"
#include "Sound.h"
#include "InputSystem.h"
#include "MessageSystem.h"
#include <ctime>

class Engine {
private:
	DWORD _lTime;
	DWORD _cTime;
	DWORD _tTime;
	DWORD _cpuRate;
	//ticks bettween updates
	int _updateTime;
	//time since last update
	double _dt;

	Graphics vFrame;
	ResourceManager resMan;
	Sound sFrame;
	InputSystem inputSys;
	MessageSystem mesSys;
	static Engine* _engine;
	Engine();
	~Engine();
public:
	void Release();
	static Engine* instance();
	void update();
	void render();
	void init(HWND hWnd,HINSTANCE hInsts, bool bWindowed);
	void reset(HWND hWnd,HINSTANCE hInsts);
	void shutdown();
	bool isLost();
	int width() {return vFrame.getWidth();}
	int height() {return vFrame.getHeight();}

	double dt() {return _dt;}
	void updatesPerSecond(int updates) {_updateTime = _cpuRate/updates;}

	float getBind(LPCSTR Name);
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
	char getFlags(LPCSTR Name);
	void setRepeat(float repeat) { inputSys.setRepeat(repeat);}

	resourceStruct* getResource(LPCSTR name, resourceType type);
	resourceStruct* getResource(LPCSTR name, D3DXCOLOR color);
	void releaseRes(LPCSTR name) {resMan.release(name);}
	//
	void addRender(renInfo& ren);
	//call this to set camera position and listener velocity
	void setCam(camera* cam, vector vel);
	//call this to set camera positions, sets listener velocity to 0,0,0
	void setCam(camera* cam);
	void setLight(int num,D3DLIGHT9& light,bool active);
	void setClearColor(D3DXCOLOR color);
	D3DLIGHT9* getLight(int num);
	void playSound(soundStruct sound, vector pos, vector vel);
	void playMusic(musicStruct stream, bool muted);
	void setVolume(float vol, soundType type);
	void clearMessages() {mesSys.clear();}
	bool getMessage(std::string message){return mesSys.getMessage(message);}
	void postMessage(std::string message){mesSys.post(message);}
	//direct access to sound and graphics, avoid using if you can
	Graphics* getvFrame() {return &vFrame;}
	Sound* getsFrame() {return &sFrame;}
	InputSystem* getInputSys() {return &inputSys;}
	int getHeight(){return vFrame.getHeight();}
	int getWidth(){return vFrame.getWidth();}
};