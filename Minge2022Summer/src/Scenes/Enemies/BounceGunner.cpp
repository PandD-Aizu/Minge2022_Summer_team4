#include"BounceGunner.hpp"

BounceGunner::BounceGunner(Point mapPos, int maxCnt)
	:Enemy(U"Sprites/bomber.png"), maxShotCnt(maxCnt)
{
	speed = 0.15;
	velocity = { 0,0 };
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
	hp = 2;
}
void BounceGunner::update() {
	shotCnt--;

	velocity = Vec2{ 0, 0 };
	if ((pos.x + speed / 2) < playerPos.x) {
		velocity.x = speed;
	}
	if ((pos.x - speed / 2) > playerPos.x) {
		velocity.x = -speed;
	}
	if ((pos.y + speed / 2) < playerPos.y) {
		velocity.y = speed;
	}
	if ((pos.y - speed / 2) > playerPos.y) {
		velocity.y = -speed;
	}

	ensureDirection();
	applyKnockBack();
	moveRestriction();
	moveNextPosition();
}
void BounceGunner::draw() const {
	Enemy::draw();
}

void BounceGunner::emitObject(Array <Object*>* objects) {
	if (shotCnt <= 0) {
		double playerDir = fmod(atan2(playerPos.y - pos.y, playerPos.x - pos.x) + Math::TwoPi, Math::TwoPi);

		*(objects) << new BounceBullet(pos, playerDir, 0.4, 0.02, 1); // バウンドする弾の発射
		shotCnt = maxShotCnt;
	}
}
