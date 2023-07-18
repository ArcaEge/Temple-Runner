#include "player.h"
#include "globals.h"

void onDropPass() {
	if (level->player->isOnPassable) {
		level->player->passingThrough = true;
		level->player->y += 1;
	}
}

Player::Player(int x, int y) : Sprite(&idle[0], x, y) {
	KeyboardManager::setOnKeyDoublePressed(2, &onDropPass);
}


// GRAVITY

void Player::gravity() {
	touchingBlock = isTouchingBlockVertically(false);
	if (!(touchingBlock && fallingspeed > 0)) {
		fallingspeed += ACCELERATION;
	}
	if (fallingspeed > 0) {
		for (int i = 0; i <= fallingspeed; i++) {
			if (isTouchingBlockVertically(true)) {
				//if (fallingspeed > 8) shouldBounce = true;
				fallingspeed = 0;
				break;
			}
			move(0, 1);
		}
	}
	else {
		for (int i = 0; i <= -fallingspeed; i++) {
			if (isTouchingBlockTop()) {
				fallingspeed = 0;
				break;
			}
			move(0, -1);
		}
	}
	touchingBlock = isTouchingBlockVertically(false);
}


// COLLISIONS

bool Player::isTouchingBlockVertically(bool spikeDamage) {
	bool isTouching = false;
	bool divideByTwo = false;
	Block* blockLeft = getBlockAt(x + 3, y + 29);
	Block* blockRight = getBlockAt(x + 15, y + 29);
	if ((blockLeft != nullptr && (blockLeft->type != 't')) || (blockRight != nullptr && (blockRight->type != 't'))) {

		if (((blockLeft == nullptr || blockLeft->type == 'w') && blockRight != nullptr && blockRight->type == 'w') ||
			((blockRight == nullptr || blockRight->type == 'w') && blockLeft != nullptr && blockLeft->type == 'w')) {

			divideByTwo = true;
		}
		else {
			if ((blockLeft != nullptr && blockLeft->y == y + 29) || (blockRight != nullptr && blockRight->y == y + 29)) {

				// Spike damage
				if (((blockLeft != nullptr && blockLeft->type == 's') || (blockRight != nullptr && blockRight->type == 's')) && !isInvincible && fallingspeed >= 0 &&spikeDamage) {
					isInvincible = true;
					health--;
					invincibleTimer = MAX_INVINCIBLE_TIMER;
				}
				if ((blockLeft != nullptr && blockLeft->type == 'p') || (blockRight != nullptr && blockRight->type == 'p')) {
					isOnPassable = true;
				}
				isTouching = true;
			}
		}

	}
	if (divideByTwo) fallingspeed /= 2;
	isInLiquid = divideByTwo;
	return isTouching && !divideByTwo;
}

bool Player::isTouchingBlockTop() {
	bool isTouching = false;
	Block* blockLeft = getBlockAt(x + 3, y, 'x');
	Block* blockRight = getBlockAt(x + 15, y, 'x');
	if (blockLeft != nullptr || blockRight != nullptr) {
		isTouching = true;
	}
	return isTouching;
}

bool Player::canMove(bool right) {
	bool canMove = true;
	Block* block = getBlockAt(x + (right ? 17 : -2), y + 28);
	Block* blockTop = getBlockAt(x + (right ? 17 : -2), y + 1, 'x');
	Block* blockLeft = getBlockAt(x + (right ? 17 : -2) - 17, y + 28);
	Block* blockRight = getBlockAt(x + (right ? 17 : -2) + 17, y + 28);
	if (blockTop != nullptr || (block != nullptr && (block->type == 'b' || block->type == 's' || block->type == 'x' || block->type == 'p') && (blockLeft == nullptr || blockLeft->type == 'w' || blockLeft->type == 't' || blockRight == nullptr || blockRight->type == 'w' || blockRight->type == 't'))) {
		canMove = false;
	}
	return canMove;
}

void Player::moveXWithCollision(int x) {
	for (int i = 0; i < abs(x); i++) {
		if (x > 0) {
			if (canMove(true)) {
				this->x++;
			}
			else {
				break;
			}
		}
		else {
			if (canMove(false)) {
				this->x--;
			}
			else {
				break;
			}
		}
	}
}

Block* Player::getBlockAt(int x, int y) {
	Block* block = nullptr;
	for (int i = 0; i < getMainLayer()->size(); i++) {
		int blockX = getMainLayer()->at(i)->x;
		int blockY = getMainLayer()->at(i)->y;
		if (x >= blockX && x < blockX + 16 && y >= blockY && y < blockY + 16) {
			block = getMainLayer()->at(i);
			break;
		}
	}
	return block;
}

Block* Player::getBlockAt(int x, int y, char type) {
	Block* block = nullptr;
	for (int i = 0; i < getMainLayer()->size(); i++) {
		int blockX = getMainLayer()->at(i)->x;
		int blockY = getMainLayer()->at(i)->y;
		if (x >= blockX && x < blockX + 16 && y >= blockY && y < blockY + 16 && getMainLayer()->at(i)->type == type) {
			block = getMainLayer()->at(i);
			break;
		}
	}
	return block;
}


// COINS

Coin* Player::getCoinAt(int x, int y) {
	Coin* coin = nullptr;
	for (int i = 0; i < getCoinLayer()->size(); i++) {
		int coinX = getCoinLayer()->at(i)->x;
		int coinY = getCoinLayer()->at(i)->y;
		if (x >= coinX && x < coinX + 16 && y >= coinY && y < coinY + 16) {
			coin = getCoinLayer()->at(i);
			break;
		}
	}
	return coin;
}

int Player::destroyTouchingCoins() {
	Coin* coinBottomLeft = getCoinAt(x + 3, y + 25);
	Coin* coinBottomRight = getCoinAt(x + 13, y + 25);
	Coin* coinTopLeft = getCoinAt(x + 3, y + 3);
	Coin* coinTopRight = getCoinAt(x + 13, y + 3);

	int destroyedCoins = 0;

	if (coinBottomLeft != nullptr && !coinBottomLeft->toBeDestroyed) {
		coinBottomLeft->destroy();
		destroyedCoins++;
	}
	if (coinBottomRight != nullptr && !coinBottomRight->toBeDestroyed) {
		coinBottomRight->destroy();
		destroyedCoins++;
	}
	if (coinTopLeft != nullptr && !coinTopLeft->toBeDestroyed) {
		coinTopLeft->destroy();
		destroyedCoins++;
	}
	if (coinTopRight != nullptr && !coinTopRight->toBeDestroyed) {
		coinTopRight->destroy();
		destroyedCoins++;
	}

	return destroyedCoins;
}


// OTHER

void Player::render() {
	if (animationActive == 0)
		sheet->Draw(x + IMG_OFFSET_X, y + IMG_OFFSET_Y);
	else if (animationActive == 1)
		sheet->Draw(x - 2 + IMG_OFFSET_X, y + IMG_OFFSET_Y);
	else {
		sheet->Draw(x + IMG_OFFSET_X, y + IMG_OFFSET_Y);
	}
}