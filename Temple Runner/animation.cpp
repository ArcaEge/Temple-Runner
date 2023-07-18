#include "animation.h"
#include "globals.h"

Animation::Animation(wchar_t* imagePath, wchar_t* imagePathFlipped, std::vector<POINT> frameCoords,
	int tileWidth, int tileHeight, int timePerFrame, bool liveRenderShader, float brightnessmult, int* sharedCurrentFrame) {
	this->imagePath = imagePath;
	this->imagePathFlipped = imagePathFlipped;
	this->frameCoords = frameCoords;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	this->timePerFrame = timePerFrame;
	this->timeLeftToNextFrame = timePerFrame;
	this->liveRenderShader = liveRenderShader;
	this->brightnessmult = brightnessmult;

	if (sharedCurrentFrame != nullptr) {
		this->pCurrentFrame = sharedCurrentFrame;
	}

	frameCount = frameCoords.size();

	for (int i = 0; i < frameCount; i++) {
		int x = frameCoords[i].x;
		int y = frameCoords[i].y;
		spriteSheets.push_back(new SpriteSheet(imagePath, global_gfx,
			new RECT({ x * tileWidth, y * tileHeight, (x * tileWidth) + tileWidth, (y * tileHeight) + tileHeight }), liveRenderShader, brightnessmult));
	}
}