# include "Player.hpp"

Player::Player() {
	// 現在の座標
	playerPosition = Vec2{ 6 * 16, 6 * 16 };
	// 現在の移動速度
	playerVelocity = Vec2{ 0, 0 };
}


//プレイヤーの移動
void Player::checkPlayerMove() {
	// 速度を0で初期化する
	playerVelocity = Vec2(0, 0);
	// キーボードで８方向移動
	if (KeyDown.pressed()) { // ↓ キー
		playerVelocity.y = playerWalkSpeed;
		playerDirection = 0;
		isPlayerMoving = true;
	}
	if (KeyUp.pressed()) { // ↑ キー
		playerVelocity.y = -playerWalkSpeed;
		playerDirection = 3;
		isPlayerMoving = true;
	}
	if (KeyLeft.pressed()) { // ← キー

		playerVelocity.x = -playerWalkSpeed;
		playerDirection = 1;
		isPlayerMoving = true;
		if (KeyDown.pressed()) playerDirection = 4; // 左下
		else if (KeyUp.pressed()) playerDirection = 5; // 左上
	}
	if (KeyRight.pressed()) { // → キー

		playerVelocity.x = playerWalkSpeed;
		playerDirection = 2;
		isPlayerMoving = true;
		if (KeyDown.pressed()) playerDirection = 6; // 右下
		else if (KeyUp.pressed()) playerDirection = 7; // 右上
	}
}


//移動制限
void Player::movementRestriction(Grid<int> mapLayer1) {
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


void Player::moveNextPosition() {
	playerPosition = nextPos;
}


void Player::drawWalk() const {
	// 歩行のアニメーションのインデックス (0, 1, 2)
	int32 animationIndex;
	if (isPlayerMoving) animationIndex = static_cast<int32>(Scene::Time() * 10 / 2) % 3;
	else animationIndex = 0;

	PlayerTexture((playerTextureSize.x * animationIndex), (playerTextureSize.y * playerDirection), playerTextureSize.x, playerTextureSize.y)
		.draw(
			  static_cast<int32>(playerPosition.x - playerTextureCenter.x),
			  static_cast<int32>(playerPosition.y - playerTextureCenter.y)
		);
}
