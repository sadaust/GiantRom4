#pragma once
#include "Engine.h"
#include "MenuSystem.h"
#include "Match.h"

#define WINDOW_TITLE "TBD GiantRom4"
#define bColor 0xFFFFFFFF
#define hColor 0xFF00FF00

class Game {
private:
	Match vs;
	char state;
	MenuSystem mainMenu;
public:
	Game();
	~Game();
	void init();
	bool update();
	void shutdown();
};