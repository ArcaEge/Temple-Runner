#pragma once

#include "sprite.h"

class Block : public Sprite {
public:
	char type = 'b'; // b = block, w = water, t = bridge top
	Block(SpriteSheet* spsheet, int x, int y) : Sprite(spsheet, x, y) {};
	Block(SpriteSheet* spsheet, int x, int y, char blocktype) : Sprite(spsheet, x, y) {
		type = blocktype;
	};

	void tick() {
		// Do nothing
	};
};