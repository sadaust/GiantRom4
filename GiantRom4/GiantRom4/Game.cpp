#include "Game.h"

void quit() {
	Engine::instance()->postMessage("quit");
}

void play() {
	Engine::instance()->postMessage("play");
}

void none() {
}


Game::Game() {

}

Game::~Game() {
	shutdown();
}

void Game::init() {
	float buttonHeight = 0.1f;
	float buttonGap = 0.02f;
	//bind mouse input for cursor movement
	Engine::instance()->bind(mouse_Xp,mouse_Xn,"MouseX");
	Engine::instance()->bind(mouse_Yp,mouse_Yn,"MouseY");
	Engine::instance()->bind(mouse_0,"Accept");
	//init main menu resorces
	mainMenu.init();
	//add single players button to main menu
	frect tRect;
	tRect.left = 0.25f;
	tRect.right = 0.75f;
	tRect.top = 0.45f;
	tRect.bottom = tRect.top+buttonHeight;
	mainMenu.addButton(play,"Puzzle",tRect,DT_CENTER|DT_VCENTER|DT_NOCLIP,bColor,hColor);
	//add multiplayer button
	tRect.top = tRect.bottom+buttonGap;
	tRect.bottom = tRect.top+buttonHeight;
	mainMenu.addButton(none,"Versus",tRect,DT_CENTER|DT_VCENTER|DT_NOCLIP,bColor,hColor);
	//add Credits button
	tRect.top = tRect.bottom+buttonGap;
	tRect.bottom = tRect.top+buttonHeight;
	mainMenu.addButton(none,"Credits",tRect,DT_CENTER|DT_VCENTER|DT_NOCLIP,bColor,hColor);
	//add quit button
	tRect.top = tRect.bottom+buttonGap;
	tRect.bottom = tRect.top+buttonHeight;
	mainMenu.addButton(quit,"Quit",tRect,DT_CENTER|DT_VCENTER|DT_NOCLIP,bColor,hColor);
	state = 0;
}

void Game::shutdown() {

}

bool Game::update() {
	switch(state) {
	case 0:
		//update and render main menu
		mainMenu.update();

		if(Engine::instance()->getMessage("quit"))
			return false;
		if(Engine::instance()->getMessage("play")) {
			vs.init();
			state = 1;
			break;
		}
		mainMenu.render();
		break;
	case 1:
		vs.update();
		vs.render();
	}
	//engine render and input clear
	Engine::instance()->render();
	Engine::instance()->clearMessages();
	//Keep alive
	return true;
}