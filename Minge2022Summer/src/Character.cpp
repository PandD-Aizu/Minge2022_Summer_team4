# include "Character.hpp"

Character::Character() {
	// 現在の座標
	playerPosition = Vec2{ 6 * 16, 6 * 16 };
	// 現在の移動速度
	playerVelocity = Vec2{ 0, 0 };
}



//キャラクターの移動
void Character::characterDirection() {
	// 速度を0で初期化する
	playerVelocity = Vec2(0, 0);


	// 前フレームのプレイヤーの向きを保存
	if (playerDirection != 4) lastPlayerDirection = playerDirection;
	// プレイヤーの向きを初期化
	playerDirection = 4;


	// キー操作によりプレイヤーに加算される速度
	Vec2 deltaPlayerVelocity{ 0, 0 };

	// キーボードで８方向移動
	if (KeyDown.pressed()) { // ↓ キー
		deltaPlayerVelocity.y += playerWalkSpeed;
		playerDirection += 1;
	}
	if (KeyUp.pressed()) { // ↑ キー
		deltaPlayerVelocity.y -= playerWalkSpeed;
		playerDirection -= 1;
	}
	if (KeyLeft.pressed()) { // ← キー

		deltaPlayerVelocity.x -= playerWalkSpeed;
		playerDirection -= 3;
	}
	if (KeyRight.pressed()) { // → キー

		deltaPlayerVelocity.x += playerWalkSpeed;
		playerDirection += 3;
	}

	// 斜め移動の際は速さを 1/√2 にする
	if (playerDirection % 2 == 0) deltaPlayerVelocity *= 1 / Math::Sqrt(2);

	// キー操作による速度を適用
	playerVelocity += deltaPlayerVelocity;

}


//移動制限
void Character::movementRestriction(Grid<int> mapLayer1) {
	// x方向の移動制限
	nextPos = playerPosition + playerVelocity * Vec2(1, 0);
	// 右方向に移動中の場合
	if (playerVelocity.x > 0) {
		// 当たり判定右上のセル座標
		Point upperRightCell(
							 static_cast<int32>((nextPos.x + playerCollisionPoint.x + playerCollisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + playerCollisionPoint.y) / MapChip::MapChipSize)
		);
		// 当たり判定の右下のセル座標
		Point lowerRightCell(
							 static_cast<int32>((nextPos.x + playerCollisionPoint.x + playerCollisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + playerCollisionPoint.y + playerCollisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 右上もしくは右下が壁に接触した場合
		if (mapLayer1[upperRightCell.y][upperRightCell.x] != 0 || mapLayer1[lowerRightCell.y][lowerRightCell.x] != 0) {
			// x座標を壁の左側の側面に矯正する
			nextPos.x = upperRightCell.x * MapChip::MapChipSize - 1 - (playerCollisionPoint.x + playerCollisionSize.x - 1);
		}
	}
	// 左方向に移動中の場合
	else {
		// 当たり判定左上のセル座標
		Point upperLeftCell(
							static_cast<int32>((nextPos.x + playerCollisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + playerCollisionPoint.y) / MapChip::MapChipSize)
		);
		// 当たり判定の左下のセル座標
		Point lowerLeftCell(
							static_cast<int32>((nextPos.x + playerCollisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + playerCollisionPoint.y + playerCollisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 左上もしくは左下が壁に接触した場合
		if (mapLayer1[upperLeftCell.y][upperLeftCell.x] != 0 || mapLayer1[lowerLeftCell.y][lowerLeftCell.x] != 0) {
			// x座標を壁の右側の側面に矯正する
			nextPos.x = (upperLeftCell.x + 1) * MapChip::MapChipSize - (playerCollisionPoint.x);

		}
	}

	// y方向の移動制限
	nextPos += playerVelocity * Vec2(0, 1);
	// 下方向に移動中の場合
	if (playerVelocity.y > 0) {
		// 当たり判定左下のセル座標
		Point lowerLeftCell(
							static_cast<int32>((nextPos.x + playerCollisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + playerCollisionPoint.y + playerCollisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 当たり判定の右下のセル座標
		Point lowerRightCell(
							 static_cast<int32>((nextPos.x + playerCollisionPoint.x + playerCollisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + playerCollisionPoint.y + playerCollisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 左下もしくは右下が壁に接触した場合
		if (mapLayer1[lowerLeftCell.y][lowerLeftCell.x] != 0 || mapLayer1[lowerRightCell.y][lowerRightCell.x] != 0) {
			// x座標を壁の左側の側面に矯正する
			nextPos.y = lowerLeftCell.y * MapChip::MapChipSize - 1 - (playerCollisionPoint.y + playerCollisionSize.y - 1);

		}
	}
	// 上方向に移動中の場合
	else {
		// 当たり判定左上のセル座標
		Point upperLeftCell(
							static_cast<int32>((nextPos.x + playerCollisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + playerCollisionPoint.y) / MapChip::MapChipSize)
		);
		// 当たり判定の右上のセル座標
		Point upperRightCell(
							 static_cast<int32>((nextPos.x + playerCollisionPoint.x + playerCollisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + playerCollisionPoint.y) / MapChip::MapChipSize)
		);
		// 左上もしくは左下が壁に接触した場合
		if (mapLayer1[upperLeftCell.y][upperLeftCell.x] != 0 || mapLayer1[upperRightCell.y][upperRightCell.x] != 0) {
			// x座標を壁の右側の側面に矯正する
			nextPos.y = (upperLeftCell.y + 1) * MapChip::MapChipSize - (playerCollisionPoint.y);

		}
	}
}


void Character::moveNextPosition() {
	playerPosition = nextPos;
}


void Character::drawWalk() const {
	// 歩行のアニメーションのインデックス(x, y)
	Vec2 animationIndex{ 0, 0 };

	// 横方向のインデックス（歩行モーション）
	if (playerDirection == 4) animationIndex.x = 0;
	else animationIndex.x = static_cast<int32>(Scene::Time() * 10 / 2) % 6;

	// 縦方向のインデックス（プレイヤーの向き）
	// プレイヤーが移動中でない場合は、最後に向いていた方向を使用する
	if (playerDirection == 4) animationIndex.y = lastPlayerDirection;
	else animationIndex.y = playerDirection;

	// 描画
	PlayerTexture((playerTextureSize.x * animationIndex.x), (playerTextureSize.y * animationIndex.y), playerTextureSize.x, playerTextureSize.y)
		.draw(
			  playerPosition.x - playerTextureCenter.x,
					playerPosition.y - playerTextureCenter.y
		);
}
