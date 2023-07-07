#pragma once

#include <wincodec.h>
#include <functional>

class KeyboardManager {
public:
	static constexpr int controls[] = { 'W', 'A', 'S', 'D', VK_SPACE }; // W, A, S, D, Space (0x20)
	static long lastPressedCountdown[5];
	static long pressedKeys[5];
	static void (*doublePressedFunc[5])(void);
	static void (*anyPressedFunc)(int);
	// static constexpr int dismissedState[] = { false, false, false, false, false };
	static const int doublePressDelay = 10;

	static bool isPressed(int key);

	static void setOnKeyDoublePressed(int key, void (*func)(void));

	static void update();
};