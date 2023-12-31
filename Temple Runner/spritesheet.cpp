#include "spritesheet.h"
#include <cmath>


double SpriteSheet::scale = NULL;
double SpriteSheet::scrolled = NULL;

extern std::vector<Light*>* getLightLayer();

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx, bool liveRenderShader, bool keepUnShaded) {
	this->gfx = gfx;
	bmp = NULL;
	this->liveRenderShader = liveRenderShader;
	this->keepUnShaded = keepUnShaded;

	loadFromImage(filename, false, nullptr);
}

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded) {
	this->gfx = gfx;
	bmp = NULL;
	this->liveRenderShader = liveRenderShader;
	this->keepUnShaded = keepUnShaded;

	loadFromImage(filename, true, croprect);
}

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded, bool isUIElement) {
	this->gfx = gfx;
	bmp = NULL;
	this->liveRenderShader = liveRenderShader;
	if (isUIElement) {
		this->isUIElement = true;
		this->keepUnShaded = true;
	}
	else {
		this->keepUnShaded = keepUnShaded;
	}

	loadFromImage(filename, true, croprect);
}

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded, int layerNo) {
	this->gfx = gfx;
	bmp = NULL;
	this->liveRenderShader = liveRenderShader;
	this->keepUnShaded = keepUnShaded;
	this->layerNo = layerNo;

	loadFromImage(filename, true, croprect);
}

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, float brightnessmult, bool flip) {
	this->gfx = gfx;
	bmp = NULL;
	this->liveRenderShader = liveRenderShader;
	this->keepUnShaded = false;
	this->brightnessmultiplier = brightnessmult;
	this->flipped = flip;

	loadFromImage(filename, true, croprect);
}

SpriteSheet::~SpriteSheet() {
	if (wicFactory) wicFactory->Release();
	if (bitmap) bitmap->Release();
}

void SpriteSheet::loadFromImage(wchar_t* filename, bool crop, RECT* croprect) {
	HRESULT hr;

	if (scale == NULL) scale = 1;
	if (scrolled == NULL) scrolled = 1;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);


	ComPtr<IWICBitmapDecoder> wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

	//if (hr != S_OK) return;

	ComPtr<IWICBitmapFrameDecode> wicFrame = NULL;
	hr = wicDecoder->GetFrame(0, &wicFrame); // O == Frame number

	//ComPtr<IWICFormatConverter> wicConverter = NULL;
	hr = wicFactory->CreateFormatConverter(&wicConverter);

	bitmap = (IWICBitmap*)wicFrame.Get();

	if (crop) {
		// Crop
		ComPtr<IWICBitmap> wicCroppedBitmap = NULL;
		if (flipped) {
			UINT width = 0, height = 0;
			bitmap->GetSize(&width, &height);
			UINT cropleft = croprect->left;
			croprect->left = width - croprect->right;
			croprect->right = width - cropleft;
		}
		hr = wicFactory->CreateBitmapFromSourceRect(bitmap.Get(), croprect->left, croprect->top, croprect->right - croprect->left, croprect->bottom - croprect->top, &wicCroppedBitmap);
		bitmap = wicCroppedBitmap;
	}

	hr = wicConverter->Initialize(bitmap.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	gfx->renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &bmp);

	wicDecoder->Release();
	wicConverter->Release();
}


void SpriteSheet::reShadeBitmap(int spriteX, int spriteY) {
	UINT width = 0, height = 0;
	bitmap->GetSize(&width, &height);

	ComPtr<IWICBitmapSource> pOriginalBitmapSource;
	bitmap->QueryInterface(IID_PPV_ARGS(&pOriginalBitmapSource));

	ComPtr<IWICBitmap> bitmap_copy = bitmap;
	wicFactory->CreateBitmapFromSource(pOriginalBitmapSource.Get(), WICBitmapCacheOnDemand, &bitmap_copy);

	ComPtr<IWICBitmapLock> pLock;
	WICRect rcLock = { 0, 0, width, height };
	bitmap_copy->Lock(&rcLock, WICBitmapLockWrite, &pLock);

	UINT bufferSize = 0;
	BYTE* pPixels;
	pLock->GetDataPointer(&bufferSize, &pPixels);

	int bytesPerPixel = 4;  // 32 bits per pixel, 8 bits per channel

	float minBrightness = 0.45f;

	float imageBrightness = minBrightness - 0.05f - (abs(layerNo) * 0.1f);
	if (layerNo == 0) {
		imageBrightness = 1.0f;
	}

	std::vector<Light*>* lightLayer = getLightLayer();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			// Get the pixel offset within the pixel buffer
			int offset = (y * width + x) * bytesPerPixel;
			if (!keepUnShaded) {
				std::vector<Light*> lightsWithinRange = std::vector<Light*>({});

				for (int i = 0; i < lightLayer->size(); i++) {
					Light* light = lightLayer->at(i);
					int distance = sqrt((spriteX + x - light->x) * (spriteX + x - light->x) + (spriteY + y - light->y) * (spriteY + y - light->y));
					if (distance <= light->radius) {
						lightsWithinRange.push_back(light);
					}
				}

				float brightness = 0, brightness_r = 1, brightness_g = 1, brightness_b = 1;
				int distance = 0;
				int radius = 0;

				for (int i = 0; i < lightsWithinRange.size(); i++) {
					Light* light = lightsWithinRange.at(i);

					int d = sqrt((spriteX + x - light->x) * (spriteX + x - light->x) + (spriteY + y - light->y) * (spriteY + y - light->y));
					// Calculate the brightness based on the distance
					float br = 1.0f - ((float)d / (light->radius / (1 - minBrightness)));
					// Adjust the brightness value to fit within the valid range (0.0 to 1.0)
					br = (((minBrightness) > ((((1.0f) < (br)) ? (1.0f) : (br)))) ? (minBrightness) : ((((1.0f) < (br)) ? (1.0f) : (br))));
					// equivalent to max(minBrightness, min(1.0f, brightness))

					float r = light->r / 255.0f;
					float g = light->g / 255.0f;
					float b = light->b / 255.0f;
					//br = round(br * 32) / 32;

					brightness_b *= (1 - b * ((float)(light->radius - d) / light->radius) / 5.0f);
					brightness_g *= (1 - g * ((float)(light->radius - d) / light->radius) / 5.0f);
					brightness_r *= (1 - r * ((float)(light->radius - d) / light->radius) / 5.0f);
					brightness += 1 - d / (float)light->radius;
				}

				brightness = round(brightness * 32) / 32;
				brightness = (((minBrightness) > ((((1.0f) < (brightness)) ? (1.0f) : (brightness)))) ? (minBrightness) : ((((1.0f) < (brightness)) ? (1.0f) : (brightness))));

				pPixels[offset + 0] = static_cast<BYTE>(pPixels[offset + 0] * minBrightness);  // Blue component
				pPixels[offset + 1] = static_cast<BYTE>(pPixels[offset + 1] * minBrightness);  // Green component
				pPixels[offset + 2] = static_cast<BYTE>(pPixels[offset + 2] * minBrightness);  // Red component

				if (lightsWithinRange.size() > 0) {
					// Set the pixel values based on the calculated brightness
					pPixels[offset + 0] = static_cast<BYTE>(min(round((255 - (255 - pPixels[offset + 0] * (brightness * (1 / minBrightness))) * brightness_b) * (255 * 32)) / (255 * 32) * imageBrightness * brightnessmultiplier, 255.0f));  // Blue component
					pPixels[offset + 1] = static_cast<BYTE>(min(round((255 - (255 - pPixels[offset + 1] * (brightness * (1 / minBrightness))) * brightness_g) * (255 * 32)) / (255 * 32) * imageBrightness * brightnessmultiplier, 255.0f));  // Green component
					pPixels[offset + 2] = static_cast<BYTE>(min(round((255 - (255 - pPixels[offset + 2] * (brightness * (1 / minBrightness))) * brightness_r) * (255 * 32)) / (255 * 32) * imageBrightness * brightnessmultiplier, 255.0f));  // Red component
				}
				
				lightsWithinRange.clear();
			}

			pPixels[offset + 3] = static_cast<BYTE>(pPixels[offset + 3] * opacity);  // Alpha component
			pPixels[offset + 0] = static_cast<BYTE>(pPixels[offset + 0] * min(1.0f, imageBrightness * 1.15f * brightnessmultiplier));  // Blue component
			pPixels[offset + 1] = static_cast<BYTE>(pPixels[offset + 1] * min(1.0f, imageBrightness * brightnessmultiplier));  // Green component
			pPixels[offset + 2] = static_cast<BYTE>(pPixels[offset + 2] * min(1.0f, imageBrightness * brightnessmultiplier));  // Red component
		}
	}

	pLock.Reset();

	wicFactory->CreateFormatConverter(&wicConverter);
	wicConverter->Initialize(bitmap_copy.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

	bmp->Release();
	gfx->renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &bmp);
	wicConverter->Release();
	bitmap_copy.Reset();
}

void SpriteSheet::setOpacity(float opacity) {
	opacity = min(max(opacity, 0.0f), 1.0f);
	if (this->opacity != opacity) {
		this->opacity = opacity;
		shaded = false;
	}
}

void SpriteSheet::Draw(double x, double y) {
	if ((liveRenderShader || !shaded) && !isUIElement) {
		shaded = true;
		reShadeBitmap(x, y);
	}

	x *= scale;
	if (!isUIElement) x -= (scrolled * (1 + layerNo / 3.0f)) * scale;
	x = floor(x);
	y *= scale;

	gfx->renderTarget->DrawBitmap(bmp,
		D2D1::RectF((double)x, (double)y, bmp->GetSize().width * scale + (double)x, bmp->GetSize().height * scale + (double)y), // Destination Rectangle
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, // Source Rectangle
		D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)
	);
}