#include "Object.hpp"

HomingBullet::HomingBullet(Vec2 pos, double startRad, double _speed, double _force)
	: Object(pos), speed(_speed), force(_force), direction(startRad)
{
}

void HomingBullet::update() {
	if (direction < 0) direction += Math::TwoPi;
	if (direction > Math::TwoPi) direction -= Math::TwoPi;

	pos.x += cos(direction) * speed;
	pos.y += sin(direction) * speed;

}

void HomingBullet::draw() const {
	Circle{ pos, 8 }.draw(Palette::Green);
}

void HomingBullet::accToPlayer(Vec2* playerPos) {
	double playerDir = fmod(atan2(playerPos->y - pos.y, playerPos->x - pos.x) + Math::TwoPi, Math::TwoPi);
	double directionDiff = fmod(playerDir - direction + Math::TwoPi, Math::TwoPi);

	if (directionDiff > Math::Pi) direction -= force;
	else direction += force;
}
