#include "Object.hpp"

BounceBullet::BounceBullet(Vec2 pos, double startRad, double _speed, double _force, int32 _bounceLimit)
	: Object(pos), speed(_speed), force(_force), direction(startRad), bounceLimit(_bounceLimit)
{
}

void BounceBullet::update() {
	if (direction < 0) direction += Math::TwoPi;
	if (direction > Math::TwoPi) direction -= Math::TwoPi;

	if(bounceAnimT-- <= 0)animIndex = 0;

	// X方向の移動先に壁があったら、directionを左右反転
	if (isTileExist(pos + Vec2{ cos(direction) * speed , 0 })) {
		if (bounceLimit == 0) destructorFlag = true;
		direction = fmod(-(direction + Math::Pi), Math::TwoPi);
		animIndex = 2;
		bounceAnimT = 8;
		bounceLimit--;
	}
	// Y方向の移動先に壁があったら、directionを上下反転
	if (isTileExist(pos + Vec2{ 0, sin(direction) * speed })) {
		if (bounceLimit == 0) destructorFlag = true;
		direction = fmod(-direction, Math::TwoPi);
		animIndex = 1;
		bounceAnimT = 8;
		bounceLimit--;
	}

	pos.x += cos(direction) * speed;
	pos.y += sin(direction) * speed;

}

void BounceBullet::draw() const {
	//Circle{ pos, 8 }.draw(Palette::Orange);
	objTexture(64 * animIndex, 64 * 3, 64, 64).drawAt(pos);
}
