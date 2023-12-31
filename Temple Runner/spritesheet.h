#pragma once

#include <vector>
#include <wincodec.h>
#include <memory>
#include <wrl/client.h>
#include "graphics.h"
#include "light.h"

using Microsoft::WRL::ComPtr;

class SpriteSheet {
	Graphics* gfx;
	ID2D1Bitmap* bmp;
	ComPtr<IWICBitmap> bitmap;
	IWICImagingFactory* wicFactory;
	IWICFormatConverter* wicConverter;

	bool shaded = false;
	bool flipped = false;
	bool liveRenderShader = false;
	bool keepUnShaded = false;
	bool isUIElement = false;
	int layerNo = 0;

	void loadFromImage(wchar_t* filename, bool crop, RECT* croprect);

public:
	static double scale;
	static double scrolled;

	float opacity = 1.0f;
	float brightnessmultiplier = 1.0f;

	SpriteSheet(wchar_t* filename, Graphics* gfx, bool liveRenderShader, bool keepUnShaded);
	SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded);
	SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded, bool isUIElement);
	SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded, int layerNo);
	SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, float brightnessmultiplier, bool flip);

	~SpriteSheet();

	//void reShadeBitmap(int spriteX, int spriteY, IWICBitmap* bitmap_optional);
	void reShadeBitmap(int spriteX, int spriteY);
	void setOpacity(float opacity);

	virtual void Draw(double x, double y);

};