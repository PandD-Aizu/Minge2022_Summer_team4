# include "Player.hpp"
# include "Math.h"

Player::Player(){
    hp=10;
	invinceT = 0;
	attackRange = MapChip::MapChipSize * 2;
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
	applyKnockBack();
	moveRestriction();
	spikeCollision();
	groundMapChipCollision();
	moveNextPosition();
	changeDirection();

	if(isAttacking()) coolT--;
}

void Player::drawHP() const {
	for (int i = 0; i < hp; i++) {
		hpTexture.draw(10 + i * 35, 20);
	}
}

void Player::detectEnemyCollision(Enemy * enm) {
	if (ArcherWall* aw = dynamic_cast<ArcherWall*>(enm)) {
	} else if (enm->pos.distanceFrom(pos) < MapChip::MapChipSize) {
		damaged();
		addKnockBack(enm->pos, enm->knockBackPower);
	}
	if (isAttacking()) {
		// 敵を攻撃したときの当たり判定。
		const double directionRad = -(directionDeg / 180) * Math::Pi;
		const Triangle hitRange = Triangle{
			pos,
			pos + Vec2{ cos(directionRad - Math::Pi / 6) * attackRange, sin(directionRad - Math::Pi / 6) * attackRange},
			pos + Vec2{ cos(directionRad + Math::Pi / 6) * attackRange, sin(directionRad + Math::Pi / 6) * attackRange}
		};
		if (hitRange.intersects(enm->pos)) {
			enm->isAttacked();
		}
	}
    if (SwordZombie* sz = dynamic_cast<SwordZombie*>(enm)) {
		if(sz->isAttacking()) {
			if (sz->pos.distanceFrom(pos) < sz->attackRange * 2) {
				if(damaged())AudioAsset(U"SZ_hit").playOneShot();
				addKnockBack(enm->pos, 10);
			}
			else {
				AudioAsset(U"SZ_attack").playOneShot();
			}
		}
    }
}


void Player::detectObjCollision(Object* obj) {
	if (Stair* stair = dynamic_cast<Stair*>(obj)) {
		
		if (stair->inPos.distanceFrom(pos) <= 10 && !(stair->reverseLock)) {
			AudioAsset(U"warp").playOneShot();
			pos = stair->outPos;
			stair->reverseLock = true;
		}
		if (stair->outPos.distanceFrom(pos) <= 10 && stair->isReversable && !(stair->reverseLock)) {
			AudioAsset(U"warp").playOneShot();
			pos = stair->inPos;
			stair->reverseLock = true;
		}

		if (stair->inPos.distanceFrom(pos) > 20 && stair->outPos.distanceFrom(pos) > 20) {
			stair->reverseLock = false;
		}
	}

	if (Bomb* bomb = dynamic_cast<Bomb*>(obj)) {
		if (bomb->state && bomb->pos.distanceFrom(pos) <= bomb->range) {
			damaged();
			addKnockBack(bomb->pos, 20);
		}
	}

	if (HomingBullet* bullet = dynamic_cast<HomingBullet*>(obj)) {
		bullet->accToPlayer(&pos);
		if (isTileExist(bullet->pos)) {
			bullet->destructorFlag = true;
			AudioAsset(U"explode").playOneShot();
		}

		if (bullet->pos.distanceFrom(pos) <= 20) {
			bullet->destructorFlag = true;
			damaged();
			addKnockBack(bullet->pos, 10);
			AudioAsset(U"explode").playOneShot();
		}
	}

	if (BounceBullet* bullet = dynamic_cast<BounceBullet*>(obj)) {
		if (bullet->pos.distanceFrom(pos) <= 20) {
			bullet->destructorFlag = true;
			damaged();
			addKnockBack(bullet->pos, 10);
			AudioAsset(U"explode").playOneShot();
		}
	}

	if (Arrow* bullet = dynamic_cast<Arrow*>(obj)) {
		if (bullet->body.intersects(Circle{ pos, 16 })) {
			bullet->destructorFlag = true;
			damaged();
			addKnockBack(bullet->pos, 10);
			AudioAsset(U"arrowHit").playOneShot();
		}
	}

	if (AccelerationFloor* floor = dynamic_cast<AccelerationFloor*>(obj)) {
		if (floor->body.intersects(pos)) {
			knockBackForce += floor->getAcceleration();
		}
	}
}

bool Player::damaged() {
	if (invinceT == 0) {
		hp--;
		invinceT = 100;
		return true;
	}
	else {
		return false;
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
	if (isAttacking()) {
		slashTexture(64 * ((10 - coolT)/2), 0, 64, 84).rotated(-ToRadians(directionDeg)+90_deg).drawAt(pos.x, pos.y);
		//Circle{ pos.x,pos.y,8 }.drawArc(-ToRadians(directionDeg) + 45_deg, 90_deg, 0, attackRange, ColorF{1,0,0,0.5});
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
	if (KeyDown.pressed() || KeyS.pressed()) { // ↓ キー
		deltaVelocity.y += walkSpeed;
		direction += 1;
	}
	if (KeyUp.pressed() || KeyW.pressed()) { // ↑ キー
		deltaVelocity.y -= walkSpeed;
		direction -= 1;
	}
	if (KeyLeft.pressed() || KeyA.pressed()) { // ← キー

		deltaVelocity.x -= walkSpeed;
		direction -= 3;
	}
	if (KeyRight.pressed() || KeyD.pressed()) { // → キー

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
	directionDeg = atan2(-(Cursor::Pos().y - Scene::Height() / 2) , Cursor::Pos().x - Scene::Width() / 2);

	if (directionDeg < 0) {
		directionDeg = directionDeg + 2 * Math::Pi;
	}

	directionDeg = directionDeg * 360 / (2 * Math::Pi);

	if ((directionDeg >= 0 && directionDeg < 22.5) || (directionDeg >= 337.5) && directionDeg < 360) {	//右向き→
		playerDirection = 7;
	}
	else if (directionDeg < 67.5) {		//右上
		playerDirection = 6;
	}
	else if (directionDeg < 112.5) {		//上
		playerDirection = 3;
	}
	else if (directionDeg < 157.5) {		//左上
		playerDirection = 0;
	}
	else if (directionDeg < 202.5) {		//左
		playerDirection = 1;
	}
	else if (directionDeg < 247.5) {		//左下
		playerDirection = 2;
	}
	else if (directionDeg < 292.5) {		//下
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
			chipIndex = 4;
		}
	}

	return chipIndex;
}


bool Player::isAttacking() const {
	if (coolT > 0) {
		return true;
	}
	else {
		return false;
	}
}

void Player::attack() {
	if (coolT <= 0) {
		AudioAsset(U"slash{}"_fmt(rand()%4)).playOneShot();
		coolT = 10;
	}
}
