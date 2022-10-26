#include "Object.hpp"

Bomb::Bomb(Vec2 _pos, SecondsF limit, int32 range)
	: Object(pos), timer{ limit }, timeLimit(limit), range(range)
{
	timer.start();
}

void Bomb::update() {
	// 爆発のカウントダウン確認
	if (state == 0 && timer.sF() == 0) {
		state = 1; // 爆発中状態に変更
		timer.set(0.5s); // 爆風の残存時間は0.5秒
		timer.start();
	}

	// 消滅判定
	if (state == 1 && timer.sF() == 0) destructorFlag = true;
}

void Bomb::draw() const {
	// 爆発待機
	if (state == 0) {
		Circle{ pos, 8 }.draw(Palette::Darkgray);
	}

	//爆発中
	if (state == 1) {
		Circle{ pos, range }.draw(Color{ Palette::Red, 100 });
	}
}
