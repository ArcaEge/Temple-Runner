#pragma once

#include "graphics.h"
#include "sprite.h"
#include "spritesheet.h"
#include "json.hpp"

#include "block.h"
#include "coin.h"
#include "player.h"
#include "light.h"

#include <vector>
#include <fstream>
#include <windows.h>

class Level {
	Graphics* gfx;
	Player* player;

public:
	std::vector<Block*> mainLayer;
	std::vector<Coin*> coinLayer;
	std::vector<Light*> lightLayer;
	std::vector<Sprite*> backgroundLayers[3]; // More big number = more far away
	std::vector<Sprite*> foregroundLayers[2]; // More big number = more close

	int scrolledDistance{};

	Level(char* levelname, Graphics* gfx);
	~Level();

	//std::vector<Block*>* getMainLayer() { return &mainLayer; };
	
	void render();
	void tick();
};