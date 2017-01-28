#include "Game.h"

Game::Game() {

}

Game::~Game() {
	shutdown();
}

void Game::init() {
	//bind mouse input for cursor movement
	Engine::instance()->bind(mouse_Xp,mouse_Xn,"MouseX");
	Engine::instance()->bind(mouse_Yp,mouse_Yn,"MouseY");
	//init main menu resorces
	mainMenu.init();
	//add a button to main menu
	frect tRect;
	tRect.left = 0.25f;
	tRect.right = 0.75f;
	tRect.top = 0.25f;
	tRect.bottom = 0.25f;
	mainMenu.addButton(0,"Nope",tRect,DT_CENTER|DT_VCENTER,0xFFFFFFFF,0XFF0000FF);
}

void Game::shutdown() {

}

bool Game::update() {
	//update and render main menu
	mainMenu.update();
	mainMenu.render();
	//engine render and input clear
	Engine::instance()->render();
	Engine::instance()->clearMessages();
	//Keep alive
	return true;
}