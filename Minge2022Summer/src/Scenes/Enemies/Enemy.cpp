#include"Enemy.hpp"


void Enemy::getPlayerPos(Vec2 _playerPos) {
	playerPos = _playerPos;
}

void Enemy::isAttacked() {
	hp--;
}

bool Enemy::isDefeated() {
	if (hp <= 0) return true;
	else return false;
}
