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
	const double JUMP_SPEED = 7.0f;
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

	bool canMove(bool right) {
		bool canMove = true;
		Block* block = getBlockAt(x + (right ? 17 : -1), y + 28);
		if (block != nullptr && (block->type == 'b' || block->type == 's')) {
			canMove = false;
		}
		return canMove;
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

	void moveXWithCollision(int x) {
		for (int i = 0; i < abs(x); i++) {
			if (x > 0) {
				if (canMove(true)) {
					this->x++;
				}
				else {
					break;
				}
			}
			else {
				if (canMove(false)) {
					this->x--;
				}
				else {
					break;
				}
			}
		}
	}

	void tick() {
		gravity();
		//if (fallingspeed >= 0) fallingspeed = 0;

		if (GetKeyState(VK_SPACE) & 0x8000) {
			if (isInLiquid)
				fallingspeed = -JUMP_SPEED / 2;
			else
				fallingspeed = -JUMP_SPEED;
		}
		if (GetKeyState('A') & 0x8000 && x > 0) {
			if (isInLiquid)
				moveXWithCollision(-1);
			else
				moveXWithCollision(-2);
		}
		if (GetKeyState('D') & 0x8000 && canMove(true)) {
			if (isInLiquid)
				moveXWithCollision(1);
			else
				moveXWithCollision(2);
		}
	};
};