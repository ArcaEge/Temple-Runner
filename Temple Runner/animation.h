#pragma once
#include "spritesheet.h"

class Animation {
	int currentFrame = 0;

public:
	wchar_t* imagePath;
	wchar_t* imagePathFlipped;
	int frameCount;
	int* pCurrentFrame = &currentFrame; // for shared animations
	int timePerFrame;
	int timeLeftToNextFrame;
	
	int tileWidth;
	int tileHeight;

	bool liveRenderShader;
	float brightnessmult;

	float opacity = 1.0f;
	float oldOpacity = opacity;

	std::vector<POINT> frameCoords;
	std::vector<SpriteSheet*> spriteSheets;
	std::vector<SpriteSheet*> spriteSheetsFlipped;

	SpriteSheet* sheet;

	Animation(wchar_t* imagePath, wchar_t* imagePathFlipped, std::vector<POINT> frameCoords, int tileWidth, int tileHeight,
		int timePerFrame, bool liveRenderShader = false, float brightnessmult = 1.0f, int* sharedCurrentFrame = nullptr);

	void tick();

	void render(int x, int y, float scale = 1.0f, float rotation = 0.0f, bool flipped = false, bool ignoreOpacity = false);
};