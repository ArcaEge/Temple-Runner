#pragma once

#include "spritesheet.h"

class HealthDisplay {
public:
	int health;
	int maxHealth;
	SpriteSheet* full;
	SpriteSheet* empty;
	int x, y;

	HealthDisplay(int health, SpriteSheet* full, SpriteSheet* empty, int x, int y) {
		this->health = health;
		this->maxHealth = health;
		this->full = full;
		this->empty = empty;
		this->x = x;
		this->y = y;
	};

	void render() {
		for (int i = 0; i < maxHealth; i++) {
			if (i < health) {
				full->Draw(x + i * 11, y);
			}
			else {
				empty->Draw(x + i * 11, y);
			}
		}
	}
};