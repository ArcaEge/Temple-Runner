#pragma once

#include <vector>
#include <wincodec.h>
#include "graphics.h"

class SpriteSheet;

class Light {
public:
	int x{}, y{}; // {} == 0
	int r{}, g{}, b{};
	int radius{};
	double* scrolled;
	double* scale;

	ID2D1Bitmap* bmp;
	IWICBitmap* bitmap;
	IWICImagingFactory* wicFactory;
	Graphics* gfx;

	Light(int x, int y, int radius, Graphics* gfx, double* scrolled, double* scale, int r, int g, int b) {
		this->x = x;
		this->y = y;
		this->radius = radius;
		this->gfx = gfx;
		this->scrolled = scrolled;
		this->scale = scale;
		this->r = r;
		this->g = g;
		this->b = b;

		CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);
		wicFactory->CreateBitmap(radius * 2, radius * 2, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnDemand, &bitmap);

		IWICBitmapLock* pLock = nullptr;
		WICRect rcLock = { 0, 0, radius * 2, radius * 2 };
		bitmap->Lock(&rcLock, WICBitmapLockWrite | WICBitmapLockRead, &pLock);

		UINT bufferSize = 0;
		BYTE* pPixels = nullptr;
		pLock->GetDataPointer(&bufferSize, &pPixels);

		int bytesPerPixel = 4;  // 32 bits per pixel, 8 bits per channel

		for (int y = 0; y < radius * 2; ++y) {
			for (int x = 0; x < radius * 2; ++x) {
				int dx = x - radius;
				int dy = y - radius;
				double distance = sqrt(dx * dx + dy * dy);

				if (distance <= radius) {
					// Get the pixel offset within the pixel buffer
					int offset = (y * (radius * 2) + x) * bytesPerPixel;

					// Divide the color channels by the alpha value (handle premultiplied alpha)
					float alpha = round(static_cast<float>(1.0f - (distance / (radius / 0.55f))) * 36) / 128 - 0.11f;
					float invAlpha = 1.0f / alpha;
					// Set the pixel values based on the calculated brightness
					pPixels[offset + 0] = static_cast<BYTE>(b);  // Blue component
					pPixels[offset + 1] = static_cast<BYTE>(g);  // Green component
					pPixels[offset + 2] = static_cast<BYTE>(r);  // Red component

					// Restore the color channels by multiplying with the alpha value
					pPixels[offset + 0] = static_cast<BYTE>(pPixels[offset + 0] * alpha);
					pPixels[offset + 1] = static_cast<BYTE>(pPixels[offset + 1] * alpha);
					pPixels[offset + 2] = static_cast<BYTE>(pPixels[offset + 2] * alpha);
				}
			}
		}

		pLock->Release();

		IWICFormatConverter* wicConverter = NULL;
		wicFactory->CreateFormatConverter(&wicConverter);
		wicConverter->Initialize(bitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

		gfx->renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &bmp);
		bitmap->Release();
		wicConverter->Release();
	}

	~Light() {

	}

	void Draw() {
		int lightX = x;
		int lightY = y;
		lightX -= *scrolled;
		lightX -= radius;
		lightY -= radius;
		lightX *= *scale;
		lightY *= *scale;

		gfx->renderTarget->DrawBitmap(bmp,
			D2D1::RectF((double)lightX, (double)lightY, bmp->GetSize().width * *scale + (double)lightX, bmp->GetSize().height * *scale + (double)lightY), // Destination Rectangle
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, // Source Rectangle
			D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)
		);
	}
};