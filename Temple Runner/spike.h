#pragma once

#include "spritesheet.h"
#include "sprite.h"

extern Graphics* global_gfx;

class Spike : public Sprite {
public:
	Spike(int x, int y) : Sprite(new SpriteSheet((wchar_t*)L"TempleTileset.png", global_gfx, new RECT({96,128,112,146}), false, false), x, y) {};

	void tick() {
		// Do nothing
	};
};