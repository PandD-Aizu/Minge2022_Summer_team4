# include "Character.hpp"

Character::Character() {
	// 現在の座標
	pos = Vec2{ 6 * 16, 6 * 16 };
	// 現在の移動速度
	velocity = Vec2{ 0, 0 };
}



//キャラクターの移動
void Character::decideDirection() {
	// 速度を0で初期化する
	velocity = Vec2(0, 0);


	// 前フレームのプレイヤーの向きを保存
	if (direction != 4) lastDirection = direction;
	// プレイヤーの向きを初期化
	direction = 4;


	// キー操作によりプレイヤーに加算される速度
	Vec2 deltaPlayerVelocity{ 0, 0 };

	// キーボードで８方向移動
	if (KeyDown.pressed()) { // ↓ キー
		deltaPlayerVelocity.y += walkSpeed;
		direction += 1;
	}
	if (KeyUp.pressed()) { // ↑ キー
		deltaPlayerVelocity.y -= walkSpeed;
		direction -= 1;
	}
	if (KeyLeft.pressed()) { // ← キー

		deltaPlayerVelocity.x -= walkSpeed;
		direction -= 3;
	}
	if (KeyRight.pressed()) { // → キー

		deltaPlayerVelocity.x += walkSpeed;
		direction += 3;
	}

	// 斜め移動の際は速さを 1/√2 にする
	if (direction % 2 == 0) deltaPlayerVelocity *= 1 / Math::Sqrt(2);

	// キー操作による速度を適用
	velocity += deltaPlayerVelocity;

}


//移動制限
void Character::moveRestriction(Grid<int> mapLayer1) {
	// x方向の移動制限
	nextPos = pos + velocity * Vec2(1, 0);
	// 右方向に移動中の場合
	if (velocity.x > 0) {
		// 当たり判定右上のセル座標
		Point upperRightCell(
							 static_cast<int32>((nextPos.x + collisionPoint.x + collisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + collisionPoint.y) / MapChip::MapChipSize)
		);
		// 当たり判定の右下のセル座標
		Point lowerRightCell(
							 static_cast<int32>((nextPos.x + collisionPoint.x + collisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + collisionPoint.y + collisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 右上もしくは右下が壁に接触した場合
		if (mapLayer1[upperRightCell.y][upperRightCell.x] == 2 || mapLayer1[lowerRightCell.y][lowerRightCell.x] == 2) {
			// x座標を壁の左側の側面に矯正する
			nextPos.x = upperRightCell.x * MapChip::MapChipSize - 1 - (collisionPoint.x + collisionSize.x - 1);
		}
	}
	// 左方向に移動中の場合
	else {
		// 当たり判定左上のセル座標
		Point upperLeftCell(
							static_cast<int32>((nextPos.x + collisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + collisionPoint.y) / MapChip::MapChipSize)
		);
		// 当たり判定の左下のセル座標
		Point lowerLeftCell(
							static_cast<int32>((nextPos.x + collisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + collisionPoint.y + collisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 左上もしくは左下が壁に接触した場合
		if (mapLayer1[upperLeftCell.y][upperLeftCell.x] == 2 || mapLayer1[lowerLeftCell.y][lowerLeftCell.x] == 2) {
			// x座標を壁の右側の側面に矯正する
			nextPos.x = (upperLeftCell.x + 1) * MapChip::MapChipSize - (collisionPoint.x);

		}
	}

	// y方向の移動制限
	nextPos += velocity * Vec2(0, 1);
	// 下方向に移動中の場合
	if (velocity.y > 0) {
		// 当たり判定左下のセル座標
		Point lowerLeftCell(
							static_cast<int32>((nextPos.x + collisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + collisionPoint.y + collisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 当たり判定の右下のセル座標
		Point lowerRightCell(
							 static_cast<int32>((nextPos.x + collisionPoint.x + collisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + collisionPoint.y + collisionSize.y - 1) / MapChip::MapChipSize)
		);
		// 左下もしくは右下が壁に接触した場合
		if (mapLayer1[lowerLeftCell.y][lowerLeftCell.x] == 2 || mapLayer1[lowerRightCell.y][lowerRightCell.x] == 2) {
			// x座標を壁の左側の側面に矯正する
			nextPos.y = lowerLeftCell.y * MapChip::MapChipSize - 1 - (collisionPoint.y + collisionSize.y - 1);

		}
	}
	// 上方向に移動中の場合
	else {
		// 当たり判定左上のセル座標
		Point upperLeftCell(
							static_cast<int32>((nextPos.x + collisionPoint.x) / MapChip::MapChipSize),
							static_cast<int32>((nextPos.y + collisionPoint.y) / MapChip::MapChipSize)
		);
		// 当たり判定の右上のセル座標
		Point upperRightCell(
							 static_cast<int32>((nextPos.x + collisionPoint.x + collisionSize.x - 1) / MapChip::MapChipSize),
							 static_cast<int32>((nextPos.y + collisionPoint.y) / MapChip::MapChipSize)
		);
		// 左上もしくは左下が壁に接触した場合
		if (mapLayer1[upperLeftCell.y][upperLeftCell.x] == 2 || mapLayer1[upperRightCell.y][upperRightCell.x] == 2) {
			// x座標を壁の右側の側面に矯正する
			nextPos.y = (upperLeftCell.y + 1) * MapChip::MapChipSize - (collisionPoint.y);

		}
	}
}

// 地面マップとの当たり判定
void Character::groundMapChipCollision(Grid<int> mapLayer0) {
	// キャラクターが載っている地面マップのセル座標
	Point cellCordinate{
		static_cast<int32>(nextPos.x / MapChip::MapChipSize),
		static_cast<int32>(nextPos.y / MapChip::MapChipSize)
	};

	// 落とし穴との当たり判定
	if (mapLayer0[cellCordinate.y][cellCordinate.x] == 6) {
		nextPos.x = (cellCordinate.x - 0.5) * MapChip::MapChipSize;
		nextPos.y = (cellCordinate.y + 0.5) * MapChip::MapChipSize;
	}
}

void Character::moveNextPosition() {
	pos = nextPos;
}



