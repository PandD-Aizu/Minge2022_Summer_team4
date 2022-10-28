#include"Bomber.hpp"

Bomber::Bomber(Point mapPos)
	:Enemy(U"Sprites/bomber.png")
{
    speed=0.15;
    velocity={0,0};
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
	hp = 3;
}
void Bomber::update(){
	plantCnt--;
	

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
void Bomber::draw() const {
	Enemy::draw();
}

void Bomber::emitObject(Array <Object *> *objects) {
	if (plantCnt <= 0) {
		*(objects) << new Bomb(pos); // 敵用の爆弾の設置
		plantCnt = 500;
	}
}
