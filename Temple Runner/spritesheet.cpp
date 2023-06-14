#include "spritesheet.h"
#include <cmath>


double SpriteSheet::scale = NULL;
double SpriteSheet::scrolled = NULL;

extern std::vector<Light*>* getLightLayer();

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx, bool liveRenderShader, bool keepUnShaded) {
	this->gfx = gfx;
	bmp = NULL;
	this->liveRenderShader = liveRenderShader;

	loadFromImage(filename, false, nullptr);
}

SpriteSheet::SpriteSheet(wchar_t* filename, Graphics* gfx, RECT* croprect, bool liveRenderShader, bool keepUnShaded) {
	this->gfx = gfx;
	bmp = NULL;
	this->liveRenderShader = liveRenderShader;
	this->keepUnShaded = keepUnShaded;

	loadFromImage(filename, true, croprect);
}

SpriteSheet::~SpriteSheet() {
	if (wicFactory) wicFactory->Release();
	if (bitmap) bitmap->Release();
	if (&bitmap) bitmap->Release();
}

void SpriteSheet::loadFromImage(wchar_t* filename, bool crop, RECT* croprect) {
	HRESULT hr;

	if (scale == NULL) scale = 1;
	if (scrolled == NULL) scrolled = 1;

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);


	IWICBitmapDecoder* wicDecoder = NULL;
	hr = wicFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

	//if (hr != S_OK) return;

	IWICBitmapFrameDecode* wicFrame = NULL;
	hr = wicDecoder->GetFrame(0, &wicFrame); // O == Frame number

	IWICFormatConverter* wicConverter = NULL;
	hr = wicFactory->CreateFormatConverter(&wicConverter);

	if (crop) {
		// Crop
		IWICBitmap* wicCroppedBitmap = NULL;
		hr = wicFactory->CreateBitmapFromSourceRect(wicFrame, croprect->left, croprect->top, croprect->right - croprect->left, croprect->bottom - croprect->top, &wicCroppedBitmap);
		bitmap = wicCroppedBitmap;
		hr = wicConverter->Initialize(wicCroppedBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	} else {
		bitmap = (IWICBitmap*)wicFrame;
		hr = wicConverter->Initialize(wicFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
	}

	gfx->renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &bmp);

	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
}


void SpriteSheet::reShadeBitmap(int spriteX, int spriteY) {
	UINT width = 0, height = 0;
	bitmap->GetSize(&width, &height);

	IWICBitmapSource* pOriginalBitmapSource = nullptr;
	bitmap->QueryInterface(IID_PPV_ARGS(&pOriginalBitmapSource));

	IWICBitmap *bitmap_copy;
	wicFactory->CreateBitmapFromSource(pOriginalBitmapSource, WICBitmapCacheOnDemand, &bitmap_copy);

	IWICBitmapLock* pLock = nullptr;
	WICRect rcLock = { 0, 0, width, height };
	bitmap_copy->Lock(&rcLock, WICBitmapLockWrite, &pLock);

	UINT bufferSize = 0;
	BYTE* pPixels = nullptr;
	pLock->GetDataPointer(&bufferSize, &pPixels);

	int bytesPerPixel = 4;  // 32 bits per pixel, 8 bits per channel

	float minBrightness = 0.45f;

	std::vector<Light*>* lightLayer = getLightLayer();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
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
				
				brightness_b *= (1 - b * ((float)(light->radius - d) / light->radius) / 2.5f);
				brightness_g *= (1 - g * ((float)(light->radius - d) / light->radius) / 2.5f);
				brightness_r *= (1 - r * ((float)(light->radius - d) / light->radius) / 2.5f);
				brightness += 1 - d/(float)light->radius;
			}

			brightness = round(brightness * 32) / 32;
			brightness = (((minBrightness) > ((((1.0f) < (brightness)) ? (1.0f) : (brightness)))) ? (minBrightness) : ((((1.0f) < (brightness)) ? (1.0f) : (brightness))));

			// Get the pixel offset within the pixel buffer
			int offset = (y * width + x) * bytesPerPixel;

			pPixels[offset + 0] = static_cast<BYTE>(pPixels[offset + 0] * minBrightness);  // Blue component
			pPixels[offset + 1] = static_cast<BYTE>(pPixels[offset + 1] * minBrightness);  // Green component
			pPixels[offset + 2] = static_cast<BYTE>(pPixels[offset + 2] * minBrightness);  // Red component

			if (lightsWithinRange.size() > 0) {
				// Set the pixel values based on the calculated brightness
				pPixels[offset + 0] = static_cast<BYTE>(min(round((255 - (255 - pPixels[offset + 0] * (brightness * 2.3f)) * brightness_b) * (255 * 32)) / (255 * 32), 255.0f));  // Blue component
				pPixels[offset + 1] = static_cast<BYTE>(min(round((255 - (255 - pPixels[offset + 1] * (brightness * 2.3f)) * brightness_g) * (255 * 32)) / (255 * 32), 255.0f));  // Green component
				pPixels[offset + 2] = static_cast<BYTE>(min(round((255 - (255 - pPixels[offset + 2] * (brightness * 2.3f)) * brightness_r) * (255 * 32)) / (255 * 32), 255.0f));  // Red component
			}
			pPixels[offset + 3] = static_cast<BYTE>(pPixels[offset + 3] * opacity);  // Alpha component
		}
	}

	pLock->Release();

	IWICFormatConverter* wicConverter = NULL;
	wicFactory->CreateFormatConverter(&wicConverter);
    wicConverter->Initialize(bitmap_copy, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

    gfx->renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &bmp);
	bitmap_copy->Release();
	wicConverter->Release();
}

void SpriteSheet::setOpacity(float opacity) {
	opacity = min(max(opacity, 0.0f), 1.0f);
	this->opacity = opacity;
	shaded = false;
}

void SpriteSheet::Draw(double x, double y) {
	if ((liveRenderShader || !shaded) && !keepUnShaded) {
		shaded = true;
		reShadeBitmap(x, y);
	}

	x -= scrolled;
	x *= scale;
	y *= scale;

	gfx->renderTarget->DrawBitmap(bmp,
		D2D1::RectF((double)x, (double)y, bmp->GetSize().width * scale + (double)x, bmp->GetSize().height * scale + (double)y), // Destination Rectangle
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, // Source Rectangle
		D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)
	);
}