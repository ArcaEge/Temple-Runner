#pragma once

#include "sprite.h"
#include "audio.h"

extern Graphics* global_gfx;
extern IXAudio2* audioEngine;

class Coin : public Sprite {
	const int ANIMATION_FRAME_MAX = 19;
	const int MAX_FRAME_TIMER = 8;
	int animationFrame = 0;
	int frameTimer = MAX_FRAME_TIMER;

	SpriteSheet coin[20] = {
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 0,0,16,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 16,0,32,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 32,0,48,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 48,0,64,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 64,0,80,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 80,0,96,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 96,0,112,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 112,0,128,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 128,0,144,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 144,0,160,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 160,0,176,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 176,0,192,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 192,0,208,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 208,0,224,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 0,0,16,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 0,0,16,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 0,0,16,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 0,0,16,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 0,0,16,16 }), false, false)),
		*(new SpriteSheet((wchar_t*)L"coin.png", global_gfx, new RECT({ 0,0,16,16 }), false, false))
	};

	IXAudio2SourceVoice* sourceVoice;

public:
	bool exists = true;
	bool toBeDestroyed = false;

	Coin(int x, int y) : Sprite(coin, x, y) {
		this->sourceVoice = Audio::LoadAudioFromFile(L"sounds/SFX/coin.wav", audioEngine);
	};

	void destroy() {
		if (toBeDestroyed) return;
		toBeDestroyed = true;
		frameTimer = 15;
		sourceVoice->Start(0);
	};

	void tick() {
		if (this->toBeDestroyed) {
			frameTimer--;
			y += 3 - frameTimer/2;
			if (frameTimer <= 0) {
				exists = false;
			}
		} else if (exists) {
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
		if (toBeDestroyed) {
			coin[animationFrame].setOpacity((float)frameTimer / 15);
		}
		if (exists) coin[animationFrame].Draw(x, y);
	}
};