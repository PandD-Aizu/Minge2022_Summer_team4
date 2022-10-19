# include "Player.hpp"
# include "Math.h"

Player::Player(){
    hp=1;
    for(int32 i=0;i<MAXENEMIESNUM;i++){
        enemiespos[i]={1000,1000};
    }
}

void Player::update(){
    for(int32 i=0;i<MAXENEMIESNUM;i++){
        if(enemiespos[i].distanceFrom(pos)<16){
            hp--;
        }
    }
	decideDirection();

	moveRestriction(mapLayer1);

	groundMapChipCollision(mapLayer0);

	moveNextPosition();

	changeDirection();
}

void Player::detectEnemyCollision(Enemy * enm) {
	if (enm->pos.distanceFrom(pos) < 16) {
		hp--;
	}
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

	animationIndex.y = direction;

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

void Player::changeDirection() {
	//プレイヤーの向きをマウスの方向に変更する関数です。
	double theta;

	theta = atan2(-(Cursor::Pos().y - Scene::Height() / 2) , Cursor::Pos().x - Scene::Width() / 2);

	if (theta < 0) {
		theta = theta + 2 * Math::Pi;
	}

	theta = theta * 360 / (2 * Math::Pi);

	if ((theta >= 0 && theta < 22.5) || (theta >= 337.5) && theta < 360) {	//右向き→
		direction = 7;
	}
	else if (theta < 67.5) {		//右上
		direction = 6;
	}
	else if (theta < 112.5) {		//上
		direction = 3;
	}
	else if (theta < 157.5) {		//左上
		direction = 0;
	}
	else if (theta < 202.5) {		//左
		direction = 1;
	}
	else if (theta < 247.5) {		//左下
		direction = 2;
	}
	else if (theta < 292.5) {		//下
		direction = 5;
	}
	else {							//右下
		direction = 8;
	}

}
