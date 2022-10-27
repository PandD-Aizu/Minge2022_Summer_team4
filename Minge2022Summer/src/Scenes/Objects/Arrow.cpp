#include "Object.hpp"

Arrow::Arrow(Vec2 pos, double startRad, double _speed)
	: Object(pos), speed(_speed), direction(startRad)
{
}

void Arrow::update() {
	if (direction < 0) direction += Math::TwoPi;
	if (direction > Math::TwoPi) direction -= Math::TwoPi;

	if (isTileExist(pos)) destructorFlag = true;

	pos.x += cos(direction) * speed;
	pos.y += sin(direction) * speed;

}

void Arrow::draw() const {
	RectF{ Arg::center(pos), 16, 4 }.draw(Palette::White);
}
