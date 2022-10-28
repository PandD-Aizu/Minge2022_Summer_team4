#include"ArcherWall.hpp"

ArcherWall::ArcherWall(Point mapPos, int maxCnt, int _direction, int delay)
	:Enemy(U"Sprites/bomber.png"), maxShotCnt(maxCnt)
{
	speed = 0.15;
	velocity = { 0,0 };
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
	hp = 3;
	direction = _direction;
	isRestless = true;
	shotCnt = delay;
}

void ArcherWall::update() {
	shotCnt--;
}

void ArcherWall::draw() const {
	Enemy::draw();
}

void ArcherWall::emitObject(Array <Object*>* objects) {
	if (shotCnt <= 0) {
		double targetDir = 0;
		if (direction == 3) targetDir = Math::Pi * 1.5;
		if (direction == 5) targetDir = Math::Pi / 2;
		if (direction == 1) targetDir = Math::Pi;
		if (direction == 7) targetDir = 0;

		*(objects) << new Arrow(pos, targetDir, 1.2); // 矢の発射
		if(playerPos.distanceFrom(pos) < 300) AudioAsset(U"arrowShot").playOneShot();
		shotCnt = maxShotCnt;
	}
}
