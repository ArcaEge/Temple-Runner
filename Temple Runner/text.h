#pragma once
#include "spritesheet.h"
#include <string>

extern Graphics* global_gfx;

class Text {
	Graphics* gfx;
	wchar_t letters[97] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!?.,:;'`\"()[]{}/\\+-%=~_|*<>^£$&#@ ";
	IWICImagingFactory* wicFactory;
	ID2D1Bitmap* bmp[97];

	void initialLoadText(wchar_t* path) {
		HRESULT hr;
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&wicFactory);

		IWICBitmapDecoder* wicDecoder = NULL;
		hr = wicFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &wicDecoder);

		IWICBitmapFrameDecode* wicFrame = NULL;
		hr = wicDecoder->GetFrame(0, &wicFrame); // O == Frame number

		IWICFormatConverter* wicConverter = NULL;
		IWICBitmap* wicCroppedBitmap = NULL;

		for (int letter = 0; letter < wcslen(letters); letter++) {
			hr = wicFactory->CreateFormatConverter(&wicConverter);

			hr = wicFactory->CreateBitmapFromSourceRect(wicFrame, letter * 10, 0, 10, 8, &wicCroppedBitmap);
			hr = wicConverter->Initialize(wicCroppedBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
			gfx->renderTarget->CreateBitmapFromWicBitmap(wicConverter, NULL, &(bmp[letter]));
		}

		if (wicDecoder) wicDecoder->Release();
		if (wicConverter) wicConverter->Release();
	}

public:
	std::wstring text;
	double size;
	int x, y;

	Text(wchar_t* path, std::wstring initialText, int x, int y, double size) {
		this->text = initialText;
		this->x = x;
		this->size = size;
		this->gfx = global_gfx;
		this->y = y;
		initialLoadText(path);
	};

	void Draw() {
		double scale = SpriteSheet::scale;

		/*x = floor(x);
		x *= scale;
		y *= scale;*/

		for (int i = 0; i < text.length(); i++) {
			for (int letter = 0; letter < wcslen(letters); letter++) {
				wchar_t a = text.at(i);
				wchar_t b = letters[letter];
				if (a == b) {
					gfx->renderTarget->DrawBitmap(bmp[letter],
						D2D1::RectF((x + (i * 8)) * scale, y * scale, (x + (8.0f * size * i) + 8.0f * size) * scale, (y + 8.0f * size) * scale), // Destination Rectangle
						1.0f,
						D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, // Source Rectangle
						D2D1::RectF(0.0f, 0.0f, 8, 8)
					);
				}
			}
		}
	}
};