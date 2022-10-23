# include "Player.hpp"
# include "Math.h"

Player::Player(){
    hp=10;
	invinceT = 0;
}

void Player::update(){
	// cb->time = static_cast<float>(Math::Sin(Scene::Time()) * 720_deg);
	cb->time = invinceT;

	if (invinceT > 0) {
		invinceT--;
		ps = blinkShader;
	}
	else {
		ps = defaultShader;
	}

	decideDirection();

	moveRestriction();

	spikeCollision();

	groundMapChipCollision();

	moveNextPosition();

	changeDirection();

	
}

void Player::detectEnemyCollision(Enemy * enm) {
	if (enm->pos.distanceFrom(pos) < 16) {
		damaged();
	}
//    if (SwordZombie* sz = dynamic_cast<SwordZombie*>(enm)) {
//      if(sz->isAttacking && sz->pos.distanceFrom(pos) < 20) {
//        damaged();
//      }
//    }
}


void Player::detectObjCollision(Object* obj) {
	if (Stair* stair = dynamic_cast<Stair*>(obj)) {
		
		if (stair->inPos.distanceFrom(pos) <= 10 && !(stair->reverseLock)) {
			pos = stair->outPos;
			stair->reverseLock = true;
		}
		if (stair->outPos.distanceFrom(pos) <= 10 && stair->isReversable && !(stair->reverseLock)) {
			pos = stair->inPos;
			stair->reverseLock = true;
		}

		if (stair->inPos.distanceFrom(pos) > 20 && stair->outPos.distanceFrom(pos) > 20) {
			stair->reverseLock = false;
		}
	}

	if (Bomb* bomb = dynamic_cast<Bomb*>(obj)) {
		if (bomb->state && bomb->position.distanceFrom(pos) <= bomb->range) {
			damaged();
		}
	}
}

void Player::damaged() {
	if (invinceT == 0) {
		hp--;
		invinceT = 100;
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
	//if (direction == 4) animationIndex.y = lastDirection;
	//else animationIndex.y = direction;

	animationIndex.y = playerDirection;
	{
		// 定数バッファを、ピクセルシェーダの定数バッファスロット 1 に設定
		Graphics2D::SetPSConstantBuffer(1, cb);

		const ScopedCustomShader2D shader{ ps };
		// 描画
		CharacterTexture((textureSize.x * animationIndex.x), (textureSize.y * animationIndex.y), textureSize.x, textureSize.y)
			.draw(pos.x - textureCenter.x, pos.y - textureCenter.y);
	}
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

void Player::changeDirection() {
	//プレイヤーの向きをマウスの方向に変更する関数です。
	double theta;

	theta = atan2(-(Cursor::Pos().y - Scene::Height() / 2) , Cursor::Pos().x - Scene::Width() / 2);

	if (theta < 0) {
		theta = theta + 2 * Math::Pi;
	}

	theta = theta * 360 / (2 * Math::Pi);

	if ((theta >= 0 && theta < 22.5) || (theta >= 337.5) && theta < 360) {	//右向き→
		playerDirection = 7;
	}
	else if (theta < 67.5) {		//右上
		playerDirection = 6;
	}
	else if (theta < 112.5) {		//上
		playerDirection = 3;
	}
	else if (theta < 157.5) {		//左上
		playerDirection = 0;
	}
	else if (theta < 202.5) {		//左
		playerDirection = 1;
	}
	else if (theta < 247.5) {		//左下
		playerDirection = 2;
	}
	else if (theta < 292.5) {		//下
		playerDirection = 5;
	}
	else {							//右下
		playerDirection = 8;
	}

}


//とげとの当たり判定を行う
void Player::spikeCollision() {
	// キャラクターが載っている地面マップのセル座標
	Point cellCordinate{
		static_cast<int32>(nextPos.x / MapChip::MapChipSize),
		static_cast<int32>(nextPos.y / MapChip::MapChipSize)
	};

	//とげとの当たり判定
	if (spike(mapLayer0[cellCordinate.y][cellCordinate.x]) == 4) {
		damaged();
	}

}

int32 Player::spike(int32 chipIndex)
{
	if (chipIndex / 10 == 1) {
		if (chipIndex % 10 != 0) {
			chipIndex = static_cast<int32>(Scene::Time() * 1 / (chipIndex % 10)) % 2 + 3;
		}
		else {
			chipIndex = 1;
		}
	}

	return chipIndex;
}
