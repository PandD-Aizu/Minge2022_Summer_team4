#include "Object.hpp"

AccelerationFloor::AccelerationFloor(Vec2 pos, double _direction, double _force)
	: Object(pos), direction(_direction), force(_force)
{
	pos = { pos.x + MapChip::MapChipSize / 2, pos.y + MapChip::MapChipSize / 2 };
	body = RectF{ Arg::center(pos), MapChip::MapChipSize }.rotated(direction);
}

void AccelerationFloor::update() {
}

void AccelerationFloor::draw() const {
	body.draw(Palette::Aqua);
	// objTexture(0, 0, 64, 64).scaled(0.5).rotated(direction + 90_deg).drawAt(pos);
}

Vec2 AccelerationFloor::getAcceleration() {
	return Vec2{ cos(direction), sin(direction) } * force;
}
