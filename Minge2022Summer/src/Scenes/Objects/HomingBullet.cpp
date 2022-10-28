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


	animIndex = Periodic::Square0_1(0.5s);
}

void HomingBullet::draw() const {
	//Circle{ pos, 8 }.draw(Palette::Green);
	objTexture(64 * animIndex, 64 * 2, 64, 64).scaled(0.5).rotated(direction + 90_deg).drawAt(pos);
}

void HomingBullet::accToPlayer(Vec2* playerPos) {
	double playerDir = fmod(atan2(playerPos->y - pos.y, playerPos->x - pos.x) + Math::TwoPi, Math::TwoPi);
	double directionDiff = fmod(playerDir - direction + Math::TwoPi, Math::TwoPi);

	if (directionDiff > Math::Pi) direction -= force;
	else direction += force;
}
