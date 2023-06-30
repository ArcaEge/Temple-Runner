#pragma once

#include "sprite.h"
#include "spritesheet.h"
#include "healthui.h"
#include <WinUser.h>

extern Graphics* global_gfx;
extern std::vector<Block*>* getMainLayer();
extern std::vector<Coin*>* getCoinLayer();
extern HealthUI* getHealthUI();

class Player : public Sprite {
	bool isInLiquid = false;
	double fallingspeed = -10;

	const int MAX_HEALTH = 3;
	int health = MAX_HEALTH;

	const int MAX_LIVES = 3;
	int lives = MAX_LIVES;

	const int MAX_INVINCIBLE_TIMER = 160;
	int invincibleTimer = MAX_INVINCIBLE_TIMER;
	bool isInvincible = false;

	const double ACCELERATION = 0.5f;
	const double JUMP_SPEED = 6.0f;

	int animationFrame = 0;
	const int ANIMATION_FRAME_MAX = 5;
	const int MAX_FRAME_TIMER = 8;
	int frameTimer = MAX_FRAME_TIMER;

	int animationActive = 0; // 0 = idle, 1 = run
	bool imageFlipped = false;

	SpriteSheet idle[6] = {
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle.png", global_gfx, new RECT({ 8,3,26,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle.png", global_gfx, new RECT({ 40,3,58,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle.png", global_gfx, new RECT({ 72,3,90,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle.png", global_gfx, new RECT({ 104,3,122,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle.png", global_gfx, new RECT({ 136,3,154,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle.png", global_gfx, new RECT({ 8,3,26,32 }), true, false))
	};
	SpriteSheet idleFlipped[6] = {
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle_Flipped.png", global_gfx, new RECT({ 8,3,26,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle_Flipped.png", global_gfx, new RECT({ 40,3,58,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle_Flipped.png", global_gfx, new RECT({ 72,3,90,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle_Flipped.png", global_gfx, new RECT({ 104,3,122,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle_Flipped.png", global_gfx, new RECT({ 136,3,154,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Idle_Flipped.png", global_gfx, new RECT({ 8,3,26,32 }), true, false))
	};

	SpriteSheet walking[6]{
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking.png", global_gfx, new RECT({ 6,3,28,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking.png", global_gfx, new RECT({ 38,3,60,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking.png", global_gfx, new RECT({ 70,3,92,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking.png", global_gfx, new RECT({ 102,3,124,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking.png", global_gfx, new RECT({ 134,3,156,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking.png", global_gfx, new RECT({ 166,3,188,32 }), true, false))
	};
	SpriteSheet walkingFlipped[6]{
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking_Flipped.png", global_gfx, new RECT({ 6,3,28,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking_Flipped.png", global_gfx, new RECT({ 38,3,60,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking_Flipped.png", global_gfx, new RECT({ 70,3,92,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking_Flipped.png", global_gfx, new RECT({ 102,3,124,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking_Flipped.png", global_gfx, new RECT({ 134,3,156,32 }), true, false)),
		*(new SpriteSheet((wchar_t*)L"Adventurer_Walking_Flipped.png", global_gfx, new RECT({ 166,3,188,32 }), true, false))
	};

public:
	int coins = 0;

	Player(int x, int y) : Sprite(&idle[0], x, y) {};

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
			for (int i = 0; i <= -fallingspeed; i++) {
				if (isTouchingBlockTop()) {
					fallingspeed = 0;
					break;
				}
				move(0, -1);
			}
		}
	}

	bool isTouchingBlockVertically() {
		bool isTouching = false;
		bool divideByTwo = false;
		Block* blockLeft = getBlockAt(x + 1, y + 29);
		Block* blockRight = getBlockAt(x + 15, y + 29);
		if ((blockLeft != nullptr && (blockLeft->type != 't')) || (blockRight != nullptr && (blockRight->type != 't'))) {

			if (((blockLeft == nullptr || blockLeft->type == 'w') && blockRight != nullptr && blockRight->type == 'w') ||
				((blockRight == nullptr || blockRight->type == 'w') && blockLeft != nullptr && blockLeft->type == 'w')) {

				divideByTwo = true;
			}
			else {
				if ((blockLeft != nullptr && blockLeft->y == y + 29) || (blockRight != nullptr && blockRight->y == y + 29)) {
					if (((blockLeft != nullptr && blockLeft->type == 's') || (blockRight != nullptr && blockRight->type == 's')) && !isInvincible) {
						isInvincible = true;
						health--;
						invincibleTimer = MAX_INVINCIBLE_TIMER;
					}
					isTouching = true;
				}
			}

		}
		if (divideByTwo) fallingspeed /= 2;
		isInLiquid = divideByTwo;
		return isTouching && !divideByTwo;
	}

	bool isTouchingBlockTop() {
		bool isTouching = false;
		Block* blockLeft = getBlockAt(x + 1, y, 'x');
		Block* blockRight = getBlockAt(x + 15, y, 'x');
		if (blockLeft != nullptr || blockRight != nullptr) {
			isTouching = true;
		}
		return isTouching;
	}

	bool canMove(bool right) {
		bool canMove = true;
		Block* block = getBlockAt(x + (right ? 16 : 0), y + 28);
		Block* blockTop = getBlockAt(x + (right ? 16 : 0), y + 1, 'x');
		Block* blockLeft = getBlockAt(x + (right ? 16 : 0) - 16, y + 28);
		Block* blockRight = getBlockAt(x + (right ? 16 : 0) + 16, y + 28);
		if (blockTop != nullptr || (block != nullptr && (block->type == 'b' || block->type == 's' || block->type == 'x') && (blockLeft == nullptr || blockLeft->type == 'w' || blockLeft->type == 't' || blockRight == nullptr || blockRight->type == 'w' || blockRight->type == 't'))) {
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

	Block* getBlockAt(int x, int y, char type) {
		Block* block = nullptr;
		for (int i = 0; i < getMainLayer()->size(); i++) {
			int blockX = getMainLayer()->at(i)->x;
			int blockY = getMainLayer()->at(i)->y;
			if (x >= blockX && x < blockX + 16 && y >= blockY && y < blockY + 16 && getMainLayer()->at(i)->type == type) {
				block = getMainLayer()->at(i);
				break;
			}
		}
		return block;
	}

	Coin* getCoinAt(int x, int y) {
		Coin* coin = nullptr;
		for (int i = 0; i < getCoinLayer()->size(); i++) {
			int coinX = getCoinLayer()->at(i)->x;
			int coinY = getCoinLayer()->at(i)->y;
			if (x >= coinX && x < coinX + 16 && y >= coinY && y < coinY + 16) {
				coin = getCoinLayer()->at(i);
				break;
			}
		}
		return coin;
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

	int destroyTouchingCoins() {
		Coin* coinBottomLeft = getCoinAt(x + 3, y + 25);
		Coin* coinBottomRight = getCoinAt(x + 13, y + 25);
		Coin* coinTopLeft = getCoinAt(x + 3, y + 3);
		Coin* coinTopRight = getCoinAt(x + 13, y + 3);

		int destroyedCoins = 0;

		if (coinBottomLeft != nullptr && !coinBottomLeft->toBeDestroyed) {
			coinBottomLeft->destroy();
			destroyedCoins++;
		}
		if (coinBottomRight != nullptr && !coinBottomRight->toBeDestroyed) {
			coinBottomRight->destroy();
			destroyedCoins++;
		}
		if (coinTopLeft != nullptr && !coinTopLeft->toBeDestroyed) {
			coinTopLeft->destroy();
			destroyedCoins++;
		}
		if (coinTopRight != nullptr && !coinTopRight->toBeDestroyed) {
			coinTopRight->destroy();
			destroyedCoins++;
		}

		return destroyedCoins;
	}

	void tick() {
		gravity();
		coins += destroyTouchingCoins();

		//if (fallingspeed >= 0) fallingspeed = 0;
		animationActive = 0;

		if (GetKeyState(VK_SPACE) & 0x8000) {
			if (isInLiquid)
				fallingspeed = -JUMP_SPEED / 2;
			else
				fallingspeed = -JUMP_SPEED;
		}
		if (GetKeyState('A') & 0x8000 && !(GetKeyState('D') & 0x8000) && x > 0) {
			animationActive = 1;
			imageFlipped = true;
			if (isInLiquid)
				moveXWithCollision(-1);
			else
				moveXWithCollision(-2);
		}
		if (GetKeyState('D') & 0x8000 && !(GetKeyState('A') & 0x8000)) {
			imageFlipped = false;
			animationActive = 1;
			if (isInLiquid)
				moveXWithCollision(1);
			else
				moveXWithCollision(2);
		}

		frameTimer--;
		if (frameTimer <= 0) {
			frameTimer = MAX_FRAME_TIMER;
			animationFrame++;
			if (animationFrame > ANIMATION_FRAME_MAX) animationFrame = 0;
		}

		if (imageFlipped) {
			if (animationActive == 0)
				sheet = &idleFlipped[animationFrame];
			else if (animationActive == 1)
				sheet = &walkingFlipped[animationFrame];
		}
		else {
			if (animationActive == 0)
				sheet = &idle[animationFrame];
			else if (animationActive == 1)
				sheet = &walking[animationFrame];
		}

		if (isInvincible) {
			if (invincibleTimer > 0) {
				invincibleTimer--;
				if ((invincibleTimer/15) % 2 == 0) {
					sheet->setOpacity(0.25f);
				}
				else {
					sheet->setOpacity(0.75f);
				}
			}
			else {
				isInvincible = false;
			}
		}
		else {
			sheet->setOpacity(1.0f);
		}

		getHealthUI()->health = health;
		getHealthUI()->coins = coins;
		getHealthUI()->lives = lives;
	}

	virtual void render() {
		if (animationActive == 0)
			sheet->Draw(x, y);
		else if (animationActive == 1)
			sheet->Draw(x - 2, y);
	}
};