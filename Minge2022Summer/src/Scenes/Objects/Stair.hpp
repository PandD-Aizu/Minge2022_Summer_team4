#pragma once
# include "../../common.hpp"

class Stair {
private:
	Vec2 inPos;
	Vec2 outPos;
	int id;
	bool isReversable;
	const int animNum = 4;
	int animIndex;

	// テクスチャ読み込み
	const Texture texture{ U"mahoujin.png" };
	// テクスチャのサイズ
	const Vec2 size{ 34, 34 };

public:
	Stair(Vec2 _inPos, Vec2 _outPos) {
		inPos = _inPos;
		outPos = _outPos;
	}

	void update(Vec2 * playerPos) {
		if (inPos.distanceFrom(*playerPos) <= 20) {
			*playerPos = outPos + Vec2{ 30,0 };
		}
		if (outPos.distanceFrom(*playerPos) <= 20 && isReversable) {
			*playerPos = inPos + Vec2{ 30,0 };
		}

		animIndex = static_cast<int32>(Scene::Time() * 10 / 2) % animNum;
	}

	void draw() const {
		texture(size.x * animIndex, 0, size.x, size.y).drawAt(inPos.x, inPos.y);
		texture(size.x * animIndex, 0, size.x, size.y).drawAt(outPos.x, outPos.y);
	}
};
