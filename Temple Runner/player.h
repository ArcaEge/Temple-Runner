#pragma once

#include "sprite.h"
#include "spritesheet.h"
#include "healthui.h"
#include "keymanager.h"
#include "coin.h"
#include "block.h"

extern Graphics* global_gfx;
extern std::vector<Block*>* getMainLayer();
extern std::vector<Coin*>* getCoinLayer();
extern HealthUI* getHealthUI();

class Player : public Sprite {
	const int MAX_HEALTH = 3;
	int health = MAX_HEALTH;

	const int MAX_LIVES = 3;
	int lives = MAX_LIVES;

	const int MAX_INVINCIBLE_TIMER = 160;
	int invincibleTimer = MAX_INVINCIBLE_TIMER;
	bool isInvincible = false;

	const double ACCELERATION = 0.5f;
	const double JUMP_SPEED = 4.0f;

	int animationIdleFrame = 0;
	int animationRunningFrame = 0;
	int animationJumpingFrame = 0;
	int animationFallingFrame = 0;

	const int ANIMATION_IDLE_MAX = 3;
	const int ANIMATION_RUNNING_MAX = 5;
	const int ANIMATION_JUMPING_MAX = 3;
	const int ANIMATION_FALLING_MAX = 1;

	const int MAX_FRAME_TIMER = 7;
	int frameTimer = MAX_FRAME_TIMER;

	const int IMG_OFFSET_X = -15;
	const int IMG_OFFSET_Y = -7;
	int animationActive = 0; // 0 = idle, 1 = run
	bool imageFlipped = false;

	SpriteSheet idle[4] = {
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 0,  0,50, 38 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 50, 0,100,38 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 100,0,150,38 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 150,0,200,38 }), true, 1.2f, false)),
	};
	SpriteSheet idleFlipped[6] = {
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 0,  0,50, 38 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 50, 0,100,38 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 100,0,150,38 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 150,0,200,38 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 200,0,250,38 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 250,0,300,38 }), true, 1.2f, true)),
	};

	SpriteSheet running[6]{
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 50, 37,100,75 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 100,37,150,75 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 150,37,200,75 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 200,37,250,75 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 250,37,300,75 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 300,37,350,75 }), true, 1.2f, false)),
	};
	SpriteSheet runningFlipped[6]{
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 50, 37,100,75 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 100,37,150,75 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 150,37,200,75 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 200,37,250,75 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 250,37,300,75 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 300,37,350,75 }), true, 1.2f, true)),
	};

	SpriteSheet jumping[4]{
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 0,  75,50, 112 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 50, 75,100,112 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 100,75,150,112 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 150,75,200,112 }), true, 1.2f, false)),

	};
	SpriteSheet jumpingFlipped[4]{
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 0,  75,50, 112 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 50, 75,100,112 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 100,75,150,112 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 150,75,200,112 }), true, 1.2f, true)),

	};

	SpriteSheet falling[2]{
		/**(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 200,75,250,112 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 250,75,300,112 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 300,75,350,112 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 1,111,50,148 }), true, 1.2f, false)),*/
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 50, 111,100,148 }), true, 1.2f, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet.png", global_gfx, new RECT({ 100,111,150,148 }), true, 1.2f, false)),
	};
	SpriteSheet fallingFlipped[2]{
		/**(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 200,75,250,112 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 250,75,300,112 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 300,75,350,112 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 1,111,50,148 }), true, 1.2f, true)),*/
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 50, 111,100,148 }), true, 1.2f, true)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\adventurer-v1.5-Sheet_Flipped.png", global_gfx, new RECT({ 100,111,150,148 }), true, 1.2f, true)),
	};

public:
	int coins = 0;
	bool isInLiquid = false;
	bool isOnPassable = false;
	bool touchingBlock = false;
	bool passingThrough = false;
	double fallingspeed = -10;

	Player(int x, int y);

	void gravity();

	bool isTouchingBlockVertically(bool spikeDamage);
	bool isTouchingBlockTop();

	bool canMove(bool right);
	void moveXWithCollision(int x);
	Block* getBlockAt(int x, int y);
	Block* getBlockAt(int x, int y, char type);

	Coin* getCoinAt(int x, int y);
	int destroyTouchingCoins();

	void tick() {
		isOnPassable = false;
		gravity();
		passingThrough = false;
		coins += destroyTouchingCoins();

		//if (fallingspeed >= 0) fallingspeed = 0;
		animationActive = 0;

		if (KeyboardManager::isPressed(4)) {
			if (isInLiquid || fallingspeed < 0 || isTouchingBlockVertically(true)) {
				if (isInLiquid)
					fallingspeed = -JUMP_SPEED / 2;
				else
					fallingspeed = -JUMP_SPEED;
			}
		}
		if (KeyboardManager::isPressed(1) && !(KeyboardManager::isPressed(3)) && x > 0) {
			animationActive = 1;
			imageFlipped = true;
			if (isInLiquid)
				moveXWithCollision(-1);
			else
				moveXWithCollision(-2);
		}
		if (KeyboardManager::isPressed(3) && !(KeyboardManager::isPressed(1))) {
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

			if (animationIdleFrame > ANIMATION_IDLE_MAX) animationIdleFrame = 0;
			if (animationRunningFrame > ANIMATION_RUNNING_MAX) animationRunningFrame = 0;
			if (animationJumpingFrame > ANIMATION_JUMPING_MAX) animationJumpingFrame = 2;
			if (animationFallingFrame > ANIMATION_FALLING_MAX) animationFallingFrame = 0;

			if (imageFlipped) {
				if (fallingspeed < 0) {
					sheet = &jumpingFlipped[animationJumpingFrame];
					animationRunningFrame = 0;
					animationFallingFrame = 0;
					animationIdleFrame = 0;
				}
				else if (fallingspeed > 0) {
					sheet = &fallingFlipped[animationFallingFrame];
					animationRunningFrame = 0;
					animationIdleFrame = 0;
					animationJumpingFrame = 0;
				}
				else if (animationActive == 0) {
					sheet = &idleFlipped[animationIdleFrame];
					animationRunningFrame = 0;
					animationJumpingFrame = 0;
					animationFallingFrame = 0;
				}
				else if (animationActive == 1) {
					sheet = &runningFlipped[animationRunningFrame];
					animationIdleFrame = 0;
					animationJumpingFrame = 0;
					animationFallingFrame = 0;
				}
			}
			else {
				if (fallingspeed < 0) {
					sheet = &jumping[animationJumpingFrame];
					animationRunningFrame = 0;
					animationFallingFrame = 0;
					animationIdleFrame = 0;
				}
				else if (fallingspeed > 0) {
					sheet = &falling[animationFallingFrame];
					animationRunningFrame = 0;
					animationIdleFrame = 0;
					animationJumpingFrame = 0;
				}
				else if (animationActive == 0) {
					sheet = &idle[animationIdleFrame];
					animationRunningFrame = 0;
					animationJumpingFrame = 0;
					animationFallingFrame = 0;
				}
				else if (animationActive == 1) {
					sheet = &running[animationRunningFrame];
					animationIdleFrame = 0;
					animationJumpingFrame = 0;
					animationFallingFrame = 0;
				}
			}

			animationIdleFrame++;
			animationRunningFrame++;
			animationJumpingFrame++;
			animationFallingFrame++;
		}

		if (isInvincible) {
			if (invincibleTimer > 0) {
				invincibleTimer--;
				if ((invincibleTimer / 15) % 2 == 0) {
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

	void render();
};