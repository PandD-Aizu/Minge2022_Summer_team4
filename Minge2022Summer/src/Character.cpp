# include "Character.hpp"
# include "common.hpp"

Character::Character() {
	// 現在の座標
	pos = Vec2{ 6 * 16, 6 * 16 };
	// 現在の移動速度
	velocity = Vec2{ 0, 0 };

	mapLayer0 = LoadCSV(U"layer0.csv");
	mapLayer1 = LoadCSV(U"layer1.csv");
}


void Character::update() {

	//decideDirection();	//キャラクターの移動関数

	moveRestriction(mapLayer1);

	groundMapChipCollision(mapLayer0);

	moveNextPosition();
}






//移動制限
void Character::moveRestriction(Grid<int> mapLayer) {
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
		if (mapLayer[upperRightCell.y][upperRightCell.x] == 2 || mapLayer[lowerRightCell.y][lowerRightCell.x] == 2) {
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
		if (mapLayer[upperLeftCell.y][upperLeftCell.x] == 2 || mapLayer[lowerLeftCell.y][lowerLeftCell.x] == 2) {
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

void Character::draw() const{
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


