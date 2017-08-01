#pragma once
#include "Player.h"
#include "Creature.h"
#include "Spawner.h"
#include "Wall.h"
#include "ChompDefines.h"



#define NUMCREATURETYPES 2
#define NUMCREATURESPRITES 4


class GameplayMan {
private:
	float gridWidth;
	float gridHeight;

	Player players[MAXIMUMPLAYERS];


	std::vector <Creature> creatures;
	std::vector <Spawner> spawners;
	std::vector <float> timetospawn; // for each spawner.
	std::vector <Wall> walls;
	std::vector <Arrow> arrows;
	
	float minTimeBetweenSpawns, maxTimeBetweenSpawns;
	float speedmultiplier;
	spriteStruct creaturesprites[NUMCREATURETYPES][NUMCREATURESPRITES];
	spriteStruct spawnersprite;
	spriteStruct wallsprite;
	spriteStruct arrowsprite[MAXIMUMPLAYERS];

	vector startpos, curpos, nextpos;
	float distancetogo;
	char creadirection;

	int curArrow[MAXIMUMPLAYERS];
	int maxActiveArrows;
	//Arrow arrows[MAXIMUMPLAYERS][MAXIMUMARROWS];
	int NumPlayers;
	int NumArrows; // number of arrows that are available in the game mode.
	double arrowdecaytime;

	void snapToNextAvailableArrow(int a_player);

	std::string inputInfo[MAXIMUMPLAYERS][8];
	std::string debugInputInfo[NUMCARDINALDIRECTIONS];

	void ClearCreatures();
	void ClearSpawners();
public:
	GameplayMan();
	void Init(float a_gridwidth, float a_gridheight, int a_numplayers, int a_numarrows, double a_arrowdecaytime, float a_minTimeBetweenSpawns, float a_maxTimeBetweenSpawns, float a_speedmultiplier, int a_maxActiveArrows);
	void Update();
	void Render();
	void setSpeed(float a_speed);
	void addSpawner(Spawner a_spawn);
	void AddArrow(int a_player, char a_rot, vector a_pos);
	bool FindNextArrow(int a_player, int & a_output);
};