#include "Object.hpp"

Bomb::Bomb(Vec2 _pos, SecondsF limit, int32 range)
	: Object(_pos), timer{ limit }, timeLimit(limit), range(range)
{
	timer.start();
}

void Bomb::update() {

	if (state == 0 )animIndex = Periodic::Square0_1(0.2s);

	// 爆発のカウントダウン確認

	if (state == 0 && timer.sF() == 0) {
		state = 1; // 爆発中状態に変更
		timer.start();
		AudioAsset(U"explode").play();
		animIndex = 0;
	}

	// 消滅判定
	if (state == 1 && animIndex == 28) destructorFlag = true;

	if (state == 1) animIndex++;;
}

void Bomb::draw() const {
	// 爆発待機
	if (state == 0) {
		Circle{ pos, 8 }.draw(Palette::Darkgray);
		objTexture(64 * animIndex, 64 * 1, 64, 64).drawAt(pos);
	}

	//爆発中
	if (state == 1) {
		// Circle{ pos, range }.draw(Color{ Palette::Red, 100 });
		explosionTexture(256 * animIndex/2, 0, 256, 256).drawAt(pos);
	}
}
