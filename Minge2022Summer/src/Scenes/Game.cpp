#include "Game.hpp"
# include "../LoadCSV.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	stairs << new Stair(Vec2{ 150, 150 }, Vec2{ 250, 150 }, true);

	mapLayer0 = LoadCSV(U"layer0.csv");
	mapLayer1 = LoadCSV(U"layer1.csv");

	if ((mapLayer0.size() != MapSize) || (mapLayer1.size() != MapSize)) {
		// MapSize と、ロードしたデータのサイズが一致しない場合のエラー
		throw Error{ U"mapLayer0: {}, mapLayer1: {}"_fmt(mapLayer0.size(), mapLayer1.size()) };
	}

	// 現在の座標
	playerPosition = Vec2{ 6 * 16, 6 * 16 };
	// 現在の移動速度
	playerVelocity = Vec2{ 0, 0 };

	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	renderTexture = RenderTexture{ 320, 240 };

	// カメラの位置と大きさを初期化
	camera.setScreen(Rect(Scene::Size()));
	camera.setScale(2);
	camera.setTargetScale(2);
	camera.setCenter(playerPosition);
}


void Game::update()
{

	// オブジェクトの状態更新
	{
		for (const auto& stair : stairs)  stair->update(&playerPosition);
	} 
	camera.update();
	camera.setCenter(playerPosition);

	//camera.update();
	//const auto t = camera.createTransformer();


	// 移動先のセル座標を設定
	//////////////////////
	// プレイヤーの移動
	//////////////////////

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

	// 移動制限処理あ
	{
		// x方向の移動制限
		Vec2 nextPos = playerPosition + playerVelocity * Vec2(1, 0);
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
		playerPosition = nextPos;
	}
	// ゲームクリア領域の当たり判定
	if (gameClearBody.intersects(Rect{
		static_cast<int32>(playerPosition.x + playerCollisionPoint.x),
		static_cast<int32>(playerPosition.y + playerCollisionPoint.y),
		static_cast<int32>(playerCollisionSize.x),
		static_cast<int32>(playerCollisionSize.y)
		}))
	{
		changeScene(U"GameClear");
	}

}

void Game::draw() const
{
	auto t = camera.createTransformer();
	//auto sv = camera.createScopedViewport();

	// renderTexture を描画先として設定
	// const ScopedRenderTarget2D rt{ renderTexture };


	// マップ
	for (int32 y = 0; y < MapSize.y; ++y)
	{
		for (int32 x = 0; x < MapSize.x; ++x)
		{
			// マップチップの描画座標
			const Point pos{ (x * MapChip::MapChipSize), (y * MapChip::MapChipSize) };

			// 地面のマップチップ
			if (const int32 chipIndex = mapLayer0[y][x];
				chipIndex != 0) // 0 の場合は描画しない
			{
				mapchip.get(chipIndex).draw(pos);
			}

			// 障害物のマップチップ
			if (const int32 chipIndex = mapLayer1[y][x];
				chipIndex != 0) // 0 の場合は描画しない
			{
				mapchip.get(chipIndex).draw(pos);
			}
		}
	}

	// ゲームクリア領域
	gameClearBody.draw(Color{ 255, 255, 0 });


	// オブジェクトの描画
	{
		for (const auto& stair : stairs)  stair->draw();
	}

	{
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

	{
		// テクスチャ拡大描画時にフィルタリング（なめらかなな拡大処理）をしないサンプラーステートを適用
		//const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

		// renderTexture を 2 倍のサイズでシーンに描画
		//renderTexture.scaled(2).draw();
	}
	{
		// 中心とズームアップ倍率の目標値をセットして、時間をかけて変更する
		//camera.setTargetCenter(Vec2{ Scene::Width()/2, Scene::Height()/2});
		//camera.setTargetScale(1.0);
	}
	//camera.draw(Palette::Orange);
}

