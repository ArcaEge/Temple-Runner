#include "level.h"

#include <string.h>

using json = nlohmann::json;

extern HWND windowhandle;

Graphics* global_gfx = nullptr;
Level* global_level = nullptr;

Level::Level(char* levelname, Graphics* graphics) {
	gfx = graphics;
	global_gfx = gfx;
	global_level = this;
	SpriteSheet::scrolled = 0;

	// Load level from file
	const char header[] = "levels/levels/";
	const char footer[] = ".ldtkl";

	char* dest;

	text = new Text((wchar_t*)L"fonts/font-outline.png", (wchar_t*)L"Hello, world!", 100, 50, 2);
	size_t len = strlen(header) + strlen(levelname) + strlen(footer)+1;
	dest = (char*)malloc(len);
	dest[len-1] = '\0';
	strcpy_s(dest, len, header);
	strcat_s(dest, len, levelname);
	strcat_s(dest, len, footer);

	std::ifstream f(dest);
	json data = json::parse(f);

	for (int layerNo = 0; layerNo < data["layerInstances"].size(); layerNo++) {
		if (data["layerInstances"][layerNo]["__identifier"] == "MainLayer") {
			addLayer(&data["layerInstances"][layerNo]["autoLayerTiles"], 0);

		}
		else if (data["layerInstances"][layerNo]["__identifier"] == "Background2") {
			addLayer(&data["layerInstances"][layerNo]["gridTiles"], -2);
		}
		else if (data["layerInstances"][layerNo]["__identifier"] == "Background1") {
			addLayer(&data["layerInstances"][layerNo]["gridTiles"], -1);
		}
		else if (data["layerInstances"][layerNo]["__identifier"] == "Foreground1") {
			addLayer(&data["layerInstances"][layerNo]["gridTiles"], 1);
		}
		else if (data["layerInstances"][layerNo]["__identifier"] == "Entities") {
			for (int entityNo = 0; entityNo < data["layerInstances"][layerNo]["entityInstances"].size(); entityNo++) {
				if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "Spike") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];
					RECT croprect = { 6*16, 8*16, 7*16, 9*16 };
					mainLayer.push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, false), x, y, 's'));

				} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "PlayerStart") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];

					player = new Player(x, y-13);
					
				} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "LightBlock") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];

					RECT croprect = { 64, 0, 80, 16 };
					mainLayer.push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, false), x, y));

				} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "Boundary") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];

					RECT croprect = { 128, 0, 144, 16 };
					mainLayer.push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, false), x, y, 'x'));

				} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "Coin") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];

					coinLayer.push_back(new Coin(x, y));

				}
				else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "Light") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];
					int radius = 64;
					int r = 0, g = 0, b = 0;

					for (int i = 0; i < data["layerInstances"][layerNo]["entityInstances"][entityNo]["fieldInstances"].size(); i++) {
						if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["fieldInstances"][i]["__identifier"] == "Radius") {
							radius = data["layerInstances"][layerNo]["entityInstances"][entityNo]["fieldInstances"][i]["__value"];

						} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["fieldInstances"][i]["__identifier"] == "Color") {
							std::string full_color = data["layerInstances"][layerNo]["entityInstances"][entityNo]["fieldInstances"][i]["__value"].get<std::string>();
							if (full_color[0] == '#')
								full_color.erase(0, 1);

							unsigned long value = stoul(full_color, nullptr, 16);
							r = (value >> 16) & 0xff;
							g = (value >> 8) & 0xff;
							b = value & 0xff;
						}
					}

					lightLayer.push_back(new Light(x, y, radius, gfx, &SpriteSheet::scrolled, &SpriteSheet::scale, r, g, b));
				}
			}
		}
	}
}

Level::~Level() {
	// Delete all sprites
}

void Level::render() {
	// Render all sprites
	for (int i = 0; i < backgroundLayers[2].size(); i++) {
		backgroundLayers[2][i]->render();
	}

	for (int i = 0; i < backgroundLayers[1].size(); i++) {
		backgroundLayers[1][i]->render();
	}

	for (int i = 0; i < lightLayer.size(); i++) {
		lightLayer[i]->Draw();
	}

	for (int i = 0; i < mainLayer.size(); i++) {
		mainLayer[i]->render();
	}

	for (int i = 0; i < coinLayer.size(); i++) {
		coinLayer[i]->render();
	}

	player->render();
	text->Draw();

	for (int i = 0; i < foregroundLayers[1].size(); i++) {
		foregroundLayers[1][i]->render();
	}
}

void Level::tick() {
	// Get window size and adjust scroll
	RECT windowSize;
	GetWindowRect(windowhandle, &windowSize);
	int width = (windowSize.right - windowSize.left)/SpriteSheet::scale;

	if (player->x - SpriteSheet::scrolled > width / 3) {
		SpriteSheet::scrolled += player->x - SpriteSheet::scrolled - (width / 3);
	} else if (player->x - SpriteSheet::scrolled < width / 5 && SpriteSheet::scrolled - ((width / 5) - (player->x - SpriteSheet::scrolled)) >= 0) {
		SpriteSheet::scrolled -= (width / 5) - (player->x - SpriteSheet::scrolled);
	}
	// Tick all sprites
	for (int i = 0; i < backgroundLayers[2].size(); i++) {
		backgroundLayers[2][i]->tick();
	}

	for (int i = 0; i < backgroundLayers[1].size(); i++) {
		backgroundLayers[1][i]->tick();
	}

	for (int i = 0; i < foregroundLayers[1].size(); i++) {
		foregroundLayers[1][i]->tick();
	}

	for (int i = 0; i < mainLayer.size(); i++) {
		mainLayer[i]->tick();
	}

	for (int i = 0; i < coinLayer.size(); i++) {
		coinLayer[i]->tick();
	}

	player->tick();
	render();
}

void Level::addLayer(json* layer, int layerNo) {
	std::vector<Block*>* layerSprites;
	bool keepUnShaded = true;

	if (layerNo == 0) {
		layerSprites = &mainLayer;
		keepUnShaded = false;
	}
	else if (layerNo < 0) {
		layerSprites = &(backgroundLayers[-layerNo]);
	}
	else {
		layerSprites = &(foregroundLayers[layerNo]);
	}

	float brightness = 0.75f - (abs(layerNo) * 0.15f);
	if (layerNo == 0) {
		brightness = 1.0f;
	}

	for (int blockNo = 0; blockNo < layer->size(); blockNo++) {
		int x = (*layer)[blockNo]["px"][0];
		int y = (*layer)[blockNo]["px"][1];

		int srcX = (*layer)[blockNo]["src"][0];
		int srcY = (*layer)[blockNo]["src"][1];

		RECT croprect = { srcX, srcY, srcX + 16, srcY + 16 };
		if (srcX >= 48 && srcX < 95 && srcY == 144) {
			layerSprites->push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, keepUnShaded, layerNo), x, y, 't'));
		}
		else if (srcX == 128 && srcY >= 128 && srcY <= 144) {
			layerSprites->push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, keepUnShaded, layerNo), x, y, 'w'));
		}
		else {
			layerSprites->push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, keepUnShaded, layerNo), x, y));
		}
	}
}