#pragma once

#include "spritesheet.h"
#include "healthdisplay.h"
#include "text.h"
#include <string>

extern Graphics* global_gfx;

class HealthUI {
public:
	int lives, maxHealth, health, coins;

	HealthDisplay* healthDisplay;
	SpriteSheet* coinIcon;
	Text* coinText;
	SpriteSheet* livesIcon;
	Text* livesText;

	HealthUI(int lives, int health) {
		this->lives = lives;
		this->health = health;
		this->maxHealth = health;
		this->coins = 0;

		RECT fullrect = { 0, 0, 16, 16 };
		RECT emptyrect = { 16, 0, 32, 16 };
		healthDisplay = new HealthDisplay(health, new SpriteSheet((wchar_t*)L"health_coin_powerups.png", global_gfx, &fullrect, false, true, true),
			new SpriteSheet((wchar_t*)L"health_coin_powerups.png", global_gfx, &emptyrect, false, true, true), 5, 5);

		coinIcon = new SpriteSheet((wchar_t*)L"health_coin_powerups.png", global_gfx, new RECT{ 64, 0, 80, 16 }, false, true, true);
		livesIcon = new SpriteSheet((wchar_t*)L"health_coin_powerups.png", global_gfx, new RECT{ 32, 0, 48, 16 }, false, true, true);

		std::string l = std::to_string(lives);
		std::wstring w_l = std::wstring(l.begin(), l.end());

		livesText = new Text((wchar_t*)L"fonts/font-outline.png", w_l, 0, 8, 1);
		coinText  = new Text((wchar_t*)L"fonts/font-outline.png", L"", 0, 8, 1);
	}

	void render(int width) {
		healthDisplay->health = health;

		healthDisplay->render();

		std::string l = std::to_string(lives);
		livesText->text = std::wstring(l.begin(), l.end());

		std::string c = std::to_string(coins);
		coinText->text = std::wstring(c.begin(), c.end());
		for (int i = 0; i < 4 - coinText->text.length(); i++) {
			coinText->text.insert(0, L"0");
		}

		livesIcon->Draw(width - 56 - 17 - 11 * livesText->text.length(), 4);
		livesText->x = width - 40 - 17 - 11 * livesText->text.length();
		livesText->Draw();

		coinIcon->Draw(width - 56, 4);
		coinText->x = width - 40;
		coinText->Draw();
	}
};