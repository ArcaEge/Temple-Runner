#pragma once

#include <Windows.h>
#include <d2d1.h>

class Graphics {
	ID2D1Factory* factory;
public:
	ID2D1HwndRenderTarget* renderTarget;
	Graphics();
	~Graphics();

	bool Init(HWND windowHandle);

	bool Resize(UINT width, UINT height);

	void BeginDraw() {
		renderTarget->BeginDraw();
	}

	void EndDraw() {
		renderTarget->EndDraw();
	}

	void ClearScreen(float r, float g, float b);
};

