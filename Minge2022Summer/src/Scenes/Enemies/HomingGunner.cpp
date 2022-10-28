#include"HomingGunner.hpp"

HomingGunner::HomingGunner(Point mapPos, int maxCnt, int _level)
	:Enemy(U"Sprites/missileBall.png"), maxShotCnt(maxCnt)
{
	speed = 0.3;
	velocity = { 0,0 };
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
	hp = 3;
	if (level == 2) hp = 5;
	level = _level;
}
void HomingGunner::update() {
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
void HomingGunner::draw() const {
	Enemy::draw();
}

void HomingGunner::emitObject(Array <Object*>* objects) {
	if (shotCnt <= 0) {
		double playerDir = fmod(atan2(playerPos.y - pos.y, playerPos.x - pos.x) + Math::TwoPi, Math::TwoPi);
		if (level == 1) {
			*(objects) << new HomingBullet(pos, playerDir, 0.8, 0.02); // 敵用の爆弾の設置
		}
		else if (level == 2) {
			*(objects) << new HomingBullet(pos, playerDir + Math::Pi * 0.75, 2, 0.1, 0.75s);
			*(objects) << new HomingBullet(pos, playerDir - Math::Pi * 0.75, 2, 0.1, 0.75s);
		}
		AudioAsset(U"homingShot").playOneShot();
		shotCnt = maxShotCnt;
	}
}
