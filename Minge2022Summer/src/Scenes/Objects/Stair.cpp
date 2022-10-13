# include "Object.hpp"

Stair::Stair(Vec2 _inPos, Vec2 _outPos, bool _isReversable) : texture(Texture{ U"mahoujin.png" }), size(Vec2{ 34,34 }), reverseLock(false) {
	inPos = _inPos;
	outPos = _outPos;
	isReversable = _isReversable;
	animNum = 4;
}

void Stair::update() {
	animIndex = static_cast<int32>(Scene::Time() * 10 / 2) % animNum;
}

void Stair::draw() const {
	texture(size.x * animIndex, 0, size.x, size.y).drawAt(inPos.x, inPos.y);
	texture(size.x * animIndex, 0, size.x, size.y).drawAt(outPos.x, outPos.y);
}
