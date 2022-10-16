# include "Player.hpp"

Player::Player(){
    hp=1;
}

void Player::update(){
	decideDirection();

	moveRestriction(mapLayer1);

	groundMapChipCollision(mapLayer0);

	moveNextPosition();
}

void Player::detectEnemyCollision(Enemy * enemy) {
	if (enemy->pos.distanceFrom(pos) < 16) {
		hp--;
	}
}

void Player::draw() const {
	// 歩行のアニメーションのインデックス(x, y)
	Vec2 animationIndex{ 0, 0 };

	// 横方向のインデックス（歩行モーション）
	if (direction == 4) animationIndex.x = 0;
	else animationIndex.x = static_cast<int32>(Scene::Time() * 10) % 6;

	// 縦方向のインデックス（プレイヤーの向き）
	// プレイヤーが移動中でない場合は、最後に向いていた方向を使用する
	if (direction == 4) animationIndex.y = lastDirection;
	else animationIndex.y = direction;

	// 描画
	CharacterTexture((textureSize.x * animationIndex.x), (textureSize.y * animationIndex.y), textureSize.x, textureSize.y)
		.draw(
			  pos.x - textureCenter.x,
					pos.y - textureCenter.y
		);
}

bool Player::died(){
    if(hp<=0){
        return true;
    }else{
        return false;
    }
}


//キャラクターの移動
void Player::decideDirection() {
	// 速度を0で初期化する
	velocity = Vec2(0, 0);


	// 前フレームのプレイヤーの向きを保存
	if (direction != 4) lastDirection = direction;
	// プレイヤーの向きを初期化
	direction = 4;


	// キー操作によりプレイヤーに加算される速度
	Vec2 deltaVelocity{ 0, 0 };

	// キーボードで８方向移動
	if (KeyDown.pressed()) { // ↓ キー
		deltaVelocity.y += walkSpeed;
		direction += 1;
	}
	if (KeyUp.pressed()) { // ↑ キー
		deltaVelocity.y -= walkSpeed;
		direction -= 1;
	}
	if (KeyLeft.pressed()) { // ← キー

		deltaVelocity.x -= walkSpeed;
		direction -= 3;
	}
	if (KeyRight.pressed()) { // → キー

		deltaVelocity.x += walkSpeed;
		direction += 3;
	}

	// 斜め移動の際は速さを 1/√2 にする
	if (direction % 2 == 0) deltaVelocity *= 1 / Math::Sqrt(2);

	// キー操作による速度を適用
	velocity += deltaVelocity;

}
