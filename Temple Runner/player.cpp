#include "player.h"
#include "globals.h"

void onPassing() {
	if (level->player->isOnPassable) {
		level->player->passingThrough = true;
		level->player->y += 1;
	}
}

Player::Player(int x, int y) : Sprite(&idle[0], x, y) {
	KeyboardManager::setOnKeyDoublePressed(2, &onPassing);
}