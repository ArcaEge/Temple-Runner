#include "sprite.h"


Sprite::Sprite(SpriteSheet* spsheet, int x, int y) {
	Sprite::sheet = spsheet;
	Sprite::x = x;
	Sprite::y = y;
}

Sprite::~Sprite() {
	sheet->~SpriteSheet();
	delete sheet;
}

void Sprite::render() {
	sheet->Draw(x, y);
}
void Sprite::move(int x, int y) {
	Sprite::x+=x;
	Sprite::y+=y;
}
void Sprite::moveTo(int x, int y) {
	Sprite::x=x;
	Sprite::y=y;
}