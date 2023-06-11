#pragma once

#include "sprite.h"
#include "spritesheet.h"
#include <WinUser.h>

extern Graphics* global_gfx;
extern std::vector<Block*>* getMainLayer();

class Player : public Sprite {
	bool isInLiquid = false;
	double fallingspeed = -10;

	const double ACCELERATION = 0.5f;
	const double JUMP_SPEED = 5.0f;
public:
	Player(int x, int y) : Sprite(new SpriteSheet((wchar_t*)L"Adventurer_Idle.png", global_gfx, new RECT({ 8,3,26,32 }), true, false), x, y) {};

	void gravity() {
		if (!(isTouchingBlockVertically() && fallingspeed > 0)) {
			fallingspeed += ACCELERATION;
		}
		if (fallingspeed > 0) {
			for (int i = 0; i <= fallingspeed; i++) {
				if (isTouchingBlockVertically()) {
					if (fallingspeed > 8) fallingspeed = -2;
					else fallingspeed = 0;
					break;
				}
				move(0, 1);
			}
		}
		else {
			move(0, fallingspeed);
		}
	}

	bool isTouchingBlockVertically() {
		bool isTouching = false;
		bool divideByTwo = false;
		/*for (int i = 0; i < getMainLayer()->size(); i++) {
			if (getMainLayer()->at(i)->y == y+29 &&
				((getMainLayer()->at(i)->x >= x && getMainLayer()->at(i)->x <= x+16) || (getMainLayer()->at(i)->x+16 >= x && getMainLayer()->at(i)->x <= x)) &&
				getMainLayer()->at(i)->type != 't') {
				if (getMainLayer()->at(i)->type == 'w') {
					divideByTwo = true;
					continue;
				}
				isTouching = true;
				break;
			}
		}*/
		Block* blockLeft = getBlockAt(x, y + 29);
		Block* blockRight = getBlockAt(x + 16, y + 29);
		if ((blockLeft != nullptr && blockLeft->type != 't') || (blockRight != nullptr && blockRight->type != 't')) {

			if (((blockLeft == nullptr || blockLeft->type == 'w') && blockRight != nullptr && blockRight->type == 'w') ||
				((blockRight == nullptr || blockRight->type == 'w') && blockLeft != nullptr && blockLeft->type == 'w')) {

				divideByTwo = true;
			}
			else {
				if ((blockLeft != nullptr && blockLeft->y == y + 29) || (blockRight != nullptr && blockRight->y == y + 29)) {
					isTouching = true;
				}
			}

		}
		if (divideByTwo) fallingspeed /= 2;
		isInLiquid = divideByTwo;
		return isTouching && !divideByTwo;
	}

	Block* getBlockAt(int x, int y) {
		Block* block = nullptr;
		for (int i = 0; i < getMainLayer()->size(); i++) {
			int blockX = getMainLayer()->at(i)->x;
			int blockY = getMainLayer()->at(i)->y;
			if (x >= blockX && x < blockX + 16 && y >= blockY && y < blockY + 16) {
				block = getMainLayer()->at(i);
				break;
			}
		}
		return block;
	}

	void tick() {
		gravity();
		//if (fallingspeed >= 0) fallingspeed = 0;

		if (GetKeyState(VK_SPACE) & 0x8000) {
			fallingspeed = -JUMP_SPEED;
		}
		if (GetKeyState('A') & 0x8000 && x > 0) {
			if (isInLiquid)
				x += -1;
			else
				x += -2;
		}
		if (GetKeyState('D') & 0x8000) {
			if (isInLiquid)
				x += 1;
			else
				x += 2;
		}
	};
};