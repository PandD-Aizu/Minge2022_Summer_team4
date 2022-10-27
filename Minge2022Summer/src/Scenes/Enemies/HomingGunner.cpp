#include"HomingGunner.hpp"

HomingGunner::HomingGunner(Point mapPos, int maxCnt)
	:Enemy(U"Sprites/bomber.png"), maxShotCnt(maxCnt)
{
	speed = 0.15;
	velocity = { 0,0 };
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
	hp = 1;
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
	moveRestriction();
	moveNextPosition();
}
void HomingGunner::draw() const {
	Enemy::draw();
}

void HomingGunner::emitObject(Array <Object*>* objects) {
	if (shotCnt <= 0) {
		*(objects) << new HomingBullet(pos, 30_deg, 0.5); // 敵用の爆弾の設置
		shotCnt = maxShotCnt;
	}
}
