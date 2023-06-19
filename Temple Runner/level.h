#pragma once

#include "graphics.h"
#include "sprite.h"
#include "spritesheet.h"
#include "json.hpp"

#include "block.h"
#include "coin.h"
#include "player.h"
#include "light.h"
#include "text.h"

#include <vector>
#include <fstream>
#include <windows.h>

class Level {
	Graphics* gfx;
	Player* player;
	Text* text;

public:
	std::vector<Block*> mainLayer;
	std::vector<Coin*> coinLayer;
	std::vector<Light*> lightLayer;
	std::vector<Block*> backgroundLayers[3]; // More big number = more far away
	std::vector<Block*> foregroundLayers[2]; // More big number = more close

	int scrolledDistance{};

	Level(char* levelname, Graphics* gfx);
	~Level();

	//std::vector<Block*>* getMainLayer() { return &mainLayer; };
	void addLayer(nlohmann::json* layer, int layerNo);
	
	void render();
	void tick();
};