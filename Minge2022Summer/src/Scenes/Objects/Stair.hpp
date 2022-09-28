#pragma once
# include "Object.hpp"

class Stair : public Object {
private:
	Vec2 inPos;
	Vec2 outPos;
	bool isReversable;
	bool reverseLock;
	const Texture texture;
	const Vec2 size;

public:
	Stair(Vec2 _inPos, Vec2 _outPos, bool _isReversable) : texture(Texture{U"mahoujin.png"}), size(Vec2{34,34}), reverseLock(false) {
		inPos = _inPos;
		outPos = _outPos;
		isReversable = _isReversable;
		animNum = 4;
	}

	void update(Vec2 * playerPos) {
		if (inPos.distanceFrom(*playerPos) <= 10 && !reverseLock) {
			*playerPos = outPos;
			reverseLock = true;
		}
		if (outPos.distanceFrom(*playerPos) <= 10 && isReversable && !reverseLock) {
			*playerPos = inPos;
			reverseLock = true;
		}

		if (inPos.distanceFrom(*playerPos) > 20 && outPos.distanceFrom(*playerPos) > 20) {
			reverseLock = false;
		}

		animIndex = static_cast<int32>(Scene::Time() * 10 / 2) % animNum;
	}

	void draw() const {
		texture(size.x * animIndex, 0, size.x, size.y).drawAt(inPos.x, inPos.y);
		texture(size.x * animIndex, 0, size.x, size.y).drawAt(outPos.x, outPos.y);
	}
};
