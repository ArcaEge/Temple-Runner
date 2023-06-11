#pragma once

#include <vector>
#include <wincodec.h>
#include "graphics.h"
#include "light.h"

class SpriteSheet {
	Graphics* gfx;
	ID2D1Bitmap* bmp;
	IWICBitmap* bitmap;
	IWICImagingFactory* wicFactory;

	bool shaded = false;
	bool liveRenderShader = false;
	bool keepUnShaded = false;

	void loadFromImage(wchar_t* filename, bool crop, RECT* croprect);

public:
	static double scale;
	static double scrolled;

	SpriteSheet(wchar_t* filename, Graphics* gfx, bool liveRenderShader, bool keepUnShaded);
	SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded);

	~SpriteSheet();

	void reShadeBitmap(int spriteX, int spriteY);

	void Draw(double x, double y);

};