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
			for (int blockNo = 0; blockNo < data["layerInstances"][layerNo]["autoLayerTiles"].size(); blockNo++) {
				int x = data["layerInstances"][layerNo]["autoLayerTiles"][blockNo]["px"][0];
				int y = data["layerInstances"][layerNo]["autoLayerTiles"][blockNo]["px"][1];

				int srcX = data["layerInstances"][layerNo]["autoLayerTiles"][blockNo]["src"][0];
				int srcY = data["layerInstances"][layerNo]["autoLayerTiles"][blockNo]["src"][1];

				RECT croprect = {srcX, srcY, srcX+16, srcY+16};
				if (srcX >= 48 && srcX < 95 && srcY == 144) {
					mainLayer.push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, false), x, y, 't'));
				} else if (srcX == 128 && srcY >= 128 && srcY <= 144) {
					mainLayer.push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, false), x, y, 'w'));
				} else {
					mainLayer.push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, false), x, y));
				}
			}

		} else if (data["layerInstances"][layerNo]["__identifier"] == "Entities") {
			for (int entityNo = 0; entityNo < data["layerInstances"][layerNo]["entityInstances"].size(); entityNo++) {
				if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "Spike") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];

					spikeLayer.push_back(new Spike(x, y));

				} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "PlayerStart") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];

					player = new Player(x, y-13);

				} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "LightBlock") {
					int x = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][0];
					int y = data["layerInstances"][layerNo]["entityInstances"][entityNo]["px"][1];

					RECT croprect = { 64, 0, 80, 16 };
					mainLayer.push_back(new Block(new SpriteSheet((wchar_t*)L"TempleTileset.png", gfx, &croprect, false, false), x, y));

				} else if (data["layerInstances"][layerNo]["entityInstances"][entityNo]["__identifier"] == "Light") {
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
	for (int i = 0; i < lightLayer.size(); i++) {
		lightLayer[i]->Draw();
	}

	for (int i = 0; i < mainLayer.size(); i++) {
		mainLayer[i]->render();
	}

	for (int i = 0; i < spikeLayer.size(); i++) {
		spikeLayer[i]->render();
	}

	player->render();
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
	for (int i = 0; i < mainLayer.size(); i++) {
		mainLayer[i]->tick();
	}

	for (int i = 0; i < spikeLayer.size(); i++) {
		spikeLayer[i]->tick();
	}

	player->tick();
	render();
}