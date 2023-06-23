#include <windows.h>
#include <chrono>
#include <time.h>
#include <WinUser.h>
#include <xaudio2.h>

#include "graphics.h"
#include "level.h"

Graphics* graphics;
Level* level;
HWND windowhandle;
IXAudio2* audioEngine;
IXAudio2MasteringVoice* masterVoice;
//AudioManager* audioManager;

std::vector<Block*>* getMainLayer() {
	return &(level->mainLayer);
}

std::vector<Light*>* getLightLayer() {
	return &(level->lightLayer);
}

std::vector<Coin*>* getCoinLayer() {
	return &(level->coinLayer);
}

HealthUI* getHealthUI() {
	return level->hui;
}

using namespace std::chrono;

double calculateScale(int width, int height) {
	// Width needs to be 30 blocks (30*16=480px), height needs to be 20 blocks (20*16=320px)
	double aspectratio = (double)width / (double)height;
	if (aspectratio > 30.0f / 20.0f) {
		// Width is bigger, fit to height
		double scale = (double)( (int)( ( (double)height / 320.0f ) * 2) ) / 2.0f; // Semi-pixel perfect (round to nearest 0.5)
		return scale;
	} else {
		// Height is bigger, fit to width
		double scale = (double)((int)(((double)width / 480.0f) * 2)) / 2.0f;
		return scale;
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;

	}
	else if (uMsg == WM_GETMINMAXINFO) {
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 300;
		lpMMI->ptMinTrackSize.y = 300;
		return 0;

	}
	else if (uMsg == WM_SIZE) {
		RECT size = { 0, 0, LOWORD(lParam), HIWORD(lParam) };
		graphics->Resize(size.right, size.bottom);
		SpriteSheet::scale = calculateScale(size.right, size.bottom);
		return 0;

	}
	else if (uMsg == WM_PAINT) { // RENDER
		graphics->BeginDraw();
		graphics->ClearScreen(0.5f, 0.5f, 0.5f);
		level->render();
		graphics->EndDraw();
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"TempleRunner";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClass(&wc);

	RECT rect = { 0, 0, 1920 / 1.5, 1080 / 1.5 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	windowhandle = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Temple Runner",               // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (!windowhandle) return -1;

	graphics = new Graphics();

	if (!graphics->Init(windowhandle)) {
		delete graphics;
		return -1;
	}

	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

	XAudio2Create(&audioEngine, 0, XAUDIO2_DEFAULT_PROCESSOR);
	audioEngine->CreateMasteringVoice(&masterVoice);

	//audioManager = new AudioManager(audioEngine);

	char levelname[] = "Level_0";
	level = new Level(levelname, graphics);

	ShowWindow(windowhandle, nCmdShow);

	SpriteSheet::scale = calculateScale(rect.right, rect.bottom);

	srand(time(NULL));

	MSG msg;
	msg.message = WM_NULL;
	uint64_t lasttime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	// wait for the next message in the queue, store the result in 'msg'
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) DispatchMessage(&msg);

		if (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - lasttime > 12) {
			lasttime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			level->tick();
		}
	}

	delete graphics;
	delete level;

	return msg.wParam;
	//return 0;
}