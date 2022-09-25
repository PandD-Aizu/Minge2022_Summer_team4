#pragma once

class Stair {
	Vec2 inPos;
	Vec2 outPos;
	int id;
	bool isReversable;

	void transportPlayer(Vec2 * playerPos) {
		if (inPos.distanceFrom(*playerPos) <= 20) {
			*playerPos = outPos + Vec2{ 30,0 };
		}

		if (outPos.distanceFrom(*playerPos) <= 20 && isReversable) {
			*playerPos = inPos + Vec2{ 30,0 };
		}
	}

	void draw() const {
		Rect{ inPos.x, inPos.y, 20, 20 }.draw(Palette::Aqua);
		Rect{ outPos.x, outPos.y, 20, 20 }.draw(Palette::Cyan);
	}
};
