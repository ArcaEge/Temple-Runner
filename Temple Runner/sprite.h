#pragma once

#include "spritesheet.h"
#include <vector>

class Block;

class Sprite {
public:
	SpriteSheet* sheet;
	int x{}, y{}; // {} == 0

	Sprite(SpriteSheet* spsheet, int x, int y);
	~Sprite();

	virtual void render();
	void move(int x, int y);
	void moveTo(int x, int y);

};