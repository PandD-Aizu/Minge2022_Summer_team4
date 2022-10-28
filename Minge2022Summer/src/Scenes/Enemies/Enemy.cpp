#include"Enemy.hpp"

Enemy::Enemy() {}

Enemy::Enemy(String texture)
	:CharacterTexture(texture) {
}

void Enemy::getPlayerPos(Vec2 _playerPos) {
	playerPos = _playerPos;
}

void Enemy::ensureDirection() {
	direction = 4;
	if (velocity.x < -0.1) direction -= 3;
	if (velocity.x > 0.1) direction += 3;
	if (velocity.y < -0.1) direction -= 1;
	if (velocity.y > 0.1) direction += 1;
	if (direction == 4) direction = lastDirection;
	lastDirection = direction;
}

void Enemy::draw() const {
	// 歩行のアニメーションのインデックス(x, y)
	Vec2 animationIndex{ 0, 0 };

	// 横方向のインデックス（歩行モーション）
	if (direction == 4) animationIndex.x = 0;
	else animationIndex.x = static_cast<int32>(Scene::Time() * 10) % 6;

	// 縦方向のインデックス（キャラクターの向き）
	// キャラクターが移動中でない場合は、最後に向いていた方向を使用する
	if (direction == 4) animationIndex.y = lastDirection;
	else animationIndex.y = direction;

	// 描画
	CharacterTexture((textureSize.x * animationIndex.x), (textureSize.y * animationIndex.y), textureSize.x, textureSize.y)
		.draw(
			  pos.x - textureCenter.x,
					pos.y - textureCenter.y
		);
}

void Enemy::isAttacked() {
	hp--;
}

bool Enemy::isDefeated() {
	if (hp <= 0) return true;
	else return false;
}

bool Enemy::isInSenceRange() const {
	if (playerPos.distanceFrom(pos) < 300) return true;
	else return false;
}
