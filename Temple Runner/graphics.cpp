#include "graphics.h"

Graphics::Graphics() {
	factory = NULL;
	renderTarget = NULL;
}

// Destructor
Graphics::~Graphics() {
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
}

bool Graphics::Init(HWND windowHandle) {
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (result != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	result = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget);

	if (result != S_OK) return false;
	return true;
}

bool Graphics::Resize(UINT width, UINT height) {
	return renderTarget->Resize(D2D1::SizeU(width, height)) == S_OK;
}

void Graphics::ClearScreen(float r, float g, float b) {
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}