#pragma once
#include "Object.hpp"


class DamageFloor : public Object {
private:
	Vec2 pos;

	const Texture texture;
	const Vec2 size;

public:
	DamageFloor(Vec2 _pos) : texture(Texture{ U"toge.png" }), size(Vec2{ 32, 32 }) {
		pos = _pos;
		animNum = 2;
	}

	void update() {
		animIndex = static_cast<int32>(Scene::Time() * 1 / 5) % animNum;

	}

	void draw() {
		texture(size.x * animIndex, 0, size.x, size.y).drawAt(pos.x, pos.y);
	}
};


