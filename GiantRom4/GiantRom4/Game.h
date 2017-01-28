#pragma once
#include "Engine.h"
#include "MenuSystem.h"

#define WINDOW_TITLE "TBD GiantRom4"

class Game {
private:
	MenuSystem mainMenu;
public:
	Game();
	~Game();
	void init();
	bool update();
	void shutdown();
};