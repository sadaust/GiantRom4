#include "Engine.h"
#include <ctime>

Engine* Engine::_engine = 0;

Engine::Engine() {

}

Engine::~Engine() {

}

Engine* Engine::instance() {
	if(!_engine) {
		_engine = new Engine;
	}
	return _engine;
}

void Engine::init(HWND hWnd,HINSTANCE hInsts, bool bWindowed) {
	sFrame.init();
	vFrame.init(hWnd,hInsts,bWindowed);
	inputSys.init(hWnd,hInsts);

	_cpuRate = CLOCKS_PER_SEC;
	_cTime = timeGetTime();
	_lTime = _cTime;
	_updateTime = _cpuRate/100;
	_tTime = _cTime + _updateTime;

}

void Engine::reset(HWND hWnd,HINSTANCE hInsts) {
	if(vFrame.restart(hWnd,hInsts)) {
		//sFrame.restart();
		//inputSys.restart(hWnd,hInsts);
		//resMan.reload();
	}
}

void Engine::update() {
	_dt = 0;
	_cTime = timeGetTime();
	if(_cTime >= _tTime) {
		_tTime = _cTime + _updateTime;
		_dt = _cTime -  _lTime;
		_dt /= (double)_cpuRate;

		inputSys.update();
		sFrame.update();

		_lTime = _cTime;
	}
}

void Engine::shutdown() {
	resMan.release();
	sFrame.shutdown();
	vFrame.shutdown();
	inputSys.shutdown();
}

bool Engine::isLost() {
	return vFrame.isLost();
}
//call this befor programm ends
void Engine::Release() {
	shutdown();
	delete _engine;
	_engine = NULL;
}

void Engine::render() {
	if(!vFrame.isLost())
		vFrame.render();
}

void Engine::addRender(renInfo& ren) {
	if(!vFrame.isLost())
		vFrame.addRender(ren);
}

//call this to set camera position and listener velocity
void Engine::setCam(camera* cam, vector vel) {
	listenProp tempProp;
	D3DXVECTOR3 tempVec;

	tempProp.pos.x = cam->cam_pos.x;
	tempProp.pos.y = cam->cam_pos.y;
	tempProp.pos.z = cam->cam_pos.z;

	tempProp.vel.x = vel.x;
	tempProp.vel.y = vel.y;
	tempProp.vel.z = vel.z;

	tempProp.up.x = cam->cam_up_vec.x;
	tempProp.up.y = cam->cam_up_vec.y;
	tempProp.up.z = cam->cam_up_vec.z;

	tempVec = cam->cam_pos - cam->cam_look_pos;
	D3DXVec3Normalize(&tempVec,&tempVec);

	tempProp.forward.x = tempVec.x;
	tempProp.forward.y = tempVec.y;
	tempProp.forward.z = tempVec.z;

	vFrame.updateCam(cam);
	sFrame.setListenProp(tempProp);
}
//call this to set camera positions, sets listener velocity to 0,0,0
void Engine::setCam(camera* cam) {
	listenProp tempProp;
	D3DXVECTOR3 tempVec;

	tempProp.pos.x = cam->cam_pos.x;
	tempProp.pos.y = cam->cam_pos.y;
	tempProp.pos.z = cam->cam_pos.z;

	tempProp.vel.x = 0;
	tempProp.vel.y = 0;
	tempProp.vel.z = 0;

	tempProp.up.x = cam->cam_up_vec.x;
	tempProp.up.y = cam->cam_up_vec.y;
	tempProp.up.z = cam->cam_up_vec.z;

	tempVec = cam->cam_pos - cam->cam_look_pos;
	D3DXVec3Normalize(&tempVec,&tempVec);

	tempProp.forward.x = tempVec.x;
	tempProp.forward.y = tempVec.y;
	tempProp.forward.z = tempVec.z;

	vFrame.updateCam(cam);
	sFrame.setListenProp(tempProp);
}

void Engine::playSound(soundStruct sound, vector pos, vector vel) {
	sFrame.play(sound,pos,vel);
}

void Engine::playMusic(musicStruct stream, bool muted) {
	sFrame.playStream(stream,muted);
}

void Engine::setVolume(float vol, soundType type){
	sFrame.setVolume(type,vol);
}

resourceStruct* Engine::getResource(LPCSTR name, resourceType type) {
	return resMan.getResource(name,type);
}

resourceStruct* Engine::getResource(LPCSTR name, D3DXCOLOR color) {
	return resMan.getResource(name,color);
}

float Engine::getBind(LPCSTR Name) {
	return inputSys.getBind(Name);
}

//create button bind
void Engine::bind(inputList input, LPCSTR name) {
	inputSys.bind(input,name);
}

//create axis bind
void Engine::bind(inputList positive, inputList negitive, LPCSTR name) {
	inputSys.bind(positive,negitive,name);
}

void Engine::clearBinds() {
	inputSys.clearBinds();
}

/*
returns input as a value between 0 and 1
mouse returns the delta for that axis in 0+ format
*/
float Engine::getAxis(inputList input) {
	return inputSys.getAxis(input);
}

float Engine::getAxis(LPCSTR Name) {
	return inputSys.getAxis(Name);
}
/*
returns button state true for pushed false for not pushed
mouse axis and sticks not recommended
*/
bool Engine::getButton(inputList input) {
	return inputSys.getButton(input);
}

bool Engine::getButton(LPCSTR Name) {
	return inputSys.getButton(Name);
}

char Engine::getFlags(LPCSTR Name) {
	return inputSys.getFlag(Name);
}

void Engine::setLight(int num,D3DLIGHT9& light,bool active) {
	vFrame.setLight(num,light,active);
}

D3DLIGHT9* Engine::getLight(int num) {
	return vFrame.getLight(num);
}

void Engine::setClearColor(D3DXCOLOR color) {
	vFrame.setClearColor(color);
}