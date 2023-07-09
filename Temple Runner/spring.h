#pragma once
#pragma once

#include "sprite.h"
#include "player.h"
#include "audio.h"

extern Graphics* global_gfx;
//extern IXAudio2* audioEngine;

class Spring : public Sprite {
	const int ANIMATION_FRAME_MAX = 7;
	const int MAX_FRAME_TIMER = 1;
	int animationFrame = 0;
	int frameTimer = MAX_FRAME_TIMER;
	int force = 15;

	SpriteSheet spring[8] = {
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 0,0,16,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 16,0,32,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 32,0,48,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 48,0,64,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 64,0,80,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 80,0,96,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 96,0,112,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"tilesets\\spring.png", global_gfx, new RECT({ 112,0,128,16 }), false, false)),
	};

	Block* collisonBlock;
	Player* player;
	// IXAudio2SourceVoice* sourceVoice;

public:
	Spring(int force, int x, int y, Player* player, std::vector<Block*>* mainLayer) : Sprite(spring, x, y) {
		this->force = force;
		this->player = player;
		RECT croprect = { 128, 0, 144, 16 };
		this->collisonBlock = new Block(new SpriteSheet((wchar_t*)L"tilesets\\TempleTileset.png", global_gfx, &croprect, false, false), x, y + 6);
		mainLayer->push_back(collisonBlock);
		//this->sourceVoice = Audio::LoadAudioFromFile(L"sounds/SFX/coin.wav", audioEngine);
	};

	void tick() {
		if (player->x - 11 <= x && x < player->x + 16 && y >= player->y + 20 && y < player->y + 28) {
			player->fallingspeed = -force;
			animationFrame++;
		}
		if (animationFrame > 0) {
			frameTimer--;
			if (frameTimer <= 0) {
				frameTimer = MAX_FRAME_TIMER;
				animationFrame++;
				if (animationFrame > ANIMATION_FRAME_MAX) {
					animationFrame = 0;
				}
			}
		}
	}

	virtual void render() {
		spring[animationFrame].Draw(x, y);
	}
};