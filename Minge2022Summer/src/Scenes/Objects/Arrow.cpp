#include "Object.hpp"

Arrow::Arrow(Vec2 pos, double startRad, double _speed)
	: Object(pos), speed(_speed), direction(startRad)
{
	body = RectF{ Arg::center(pos), MapChip::MapChipSize, MapChip::MapChipSize / 4 }.rotated(direction);
}

void Arrow::update() {
	if (direction < 0) direction += Math::TwoPi;
	if (direction > Math::TwoPi) direction -= Math::TwoPi;

	if (isTileExist(pos)) destructorFlag = true;

	pos.x += cos(direction) * speed;
	pos.y += sin(direction) * speed;
	body = RectF{ Arg::center(pos), MapChip::MapChipSize, MapChip::MapChipSize / 4 }.rotated(direction);
}

void Arrow::draw() const {
	objTexture(0, 0, 64, 64).rotated(direction + 90_deg).drawAt(pos);
}
