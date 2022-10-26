#include "Object.hpp"

HomingBullet::HomingBullet(Vec2 pos, double _speed, double _force, double startRad)
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
	double playerDir = atan2(playerPos->x - pos.x, playerPos->y - pos.y);
	double directionDiff = fmod(playerDir - direction, Math::TwoPi);

	if (directionDiff > Math::Pi) direction += 0.02;
	else direction -= 0.02;
}
