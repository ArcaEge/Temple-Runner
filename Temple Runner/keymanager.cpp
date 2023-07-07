#include "keymanager.h"

void nullfunc() {}

long KeyboardManager::lastPressedCountdown[] = { -1, -1, -1, -1, -1 };
long KeyboardManager::pressedKeys[] = {false, false, false, false, false};
void (*nfunc)(void) = nullfunc;
void (*KeyboardManager::doublePressedFunc[5])(void) = { nfunc, nfunc, nfunc, nfunc, nfunc };

void KeyboardManager::update() {
	for (int i = 0; i < 5; i++) {
		bool keystate = GetKeyState(controls[i]) & 0x8000;

		lastPressedCountdown[i]--;

		if (KeyboardManager::pressedKeys[i] && !keystate) {     // Released
			lastPressedCountdown[i] = doublePressDelay;         // Reset countdown
		}
		if (!KeyboardManager::pressedKeys[i] && keystate) {
			for (int j = 0; j < 5; j++) {
				if (j == i) continue;
				lastPressedCountdown[j] = -1;
			}
		}
		if (lastPressedCountdown[i] >= 0 && keystate) {         // Double pressed
			(KeyboardManager::doublePressedFunc[i])();          // Call function
		}

		KeyboardManager::pressedKeys[i] = keystate;
	}
}

bool KeyboardManager::isPressed(int key) {
	return KeyboardManager::pressedKeys[key];
}

void KeyboardManager::setOnKeyDoublePressed(int key, void (*func)(void)) {
	KeyboardManager::doublePressedFunc[key] = func;
}