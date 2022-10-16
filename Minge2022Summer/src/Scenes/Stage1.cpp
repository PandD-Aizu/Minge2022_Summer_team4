#include "Stage1.hpp"
//# include "../LoadCSV.hpp"

Stage1::Stage1(const InitData& init)
	: IScene{ init }
{
    // マップのセルの数（横 20, 縦 15 マス）
    MapSize = Size{26, 50};
    
	stairs << new Stair(Vec2{ 150, 150 }, Vec2{ 250, 600 }, true);
    mapLayer0 = LoadCSV(U"maps/stage1/layer0.csv");
    mapLayer1 = LoadCSV(U"maps/stage1/layer1.csv");
    
  

	// layer1上の敵を読み込む
	for (int32 y = 0; y < MapSize.y; ++y)
	{
		for (int32 x = 0; x < MapSize.x; ++x)
		{
			const Point pos{ (x * MapChip::MapChipSize), (y * MapChip::MapChipSize) };
			switch(mapLayer1[y][x])
			{
			case 5:
				enemies << new SwordZombie(pos);
				break;
			}
		}
	}

	if ((mapLayer0.size() != MapSize) || (mapLayer1.size() != MapSize)) {
		// MapSize と、ロードしたデータのサイズが一致しない場合のエラー
		throw Error{ U"mapLayer0: {}, mapLayer1: {}"_fmt(mapLayer0.size(), mapLayer1.size()) };
	}
	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	renderTexture = RenderTexture{ 320, 240 };

	// カメラの位置と大きさを初期化
	camera.setScreen(Rect(Scene::Size()));
	camera.setScale(2);
	camera.setTargetScale(2);
	camera.setCenter(player.pos);
	camera.setFollowingSpeed(0.05);
}


void Stage1::update()
{
	// オブジェクトの状態更新
	{
		for (const auto& stair : stairs)  stair->update(&player.pos);
	}

	// カメラの移動
	{
		Vec2 cursorPos{ Cursor::Pos() };

		if (cursorPos.x < 0) cursorPos.x = 0;
		if (cursorPos.x > Scene::Width()) cursorPos.x = Scene::Width();
		if (cursorPos.y < 0) cursorPos.y = 0;
		if (cursorPos.y > Scene::Height()) cursorPos.y = Scene::Height();

		camera.setTargetCenter(
			player.pos + (cursorPos - Vec2(Scene::Width() / 2, Scene::Height() / 2)) / 5
		);
	}

	camera.update();

	// プレイヤーの状態更新
	player.update();


	//for (int32 i = 0; i < countswordzombies; i++) {
	//	enemiespos[i] = swordzombie[i].sendmypos();
	//}

	for (auto& enemy : enemies) {
		enemy->getPlayerPos(player.pos);
		enemy->update();
		player.detectEnemyCollision(enemy);
	}

	// ゲームクリア領域の当たり判定
	if (gameClearBody.intersects(Rect{
		static_cast<int32>(player.pos.x + player.collisionPoint.x),
		static_cast<int32>(player.pos.y + player.collisionPoint.y),
		static_cast<int32>(player.collisionSize.x),
		static_cast<int32>(player.collisionSize.y)
		}))
	{
		changeScene(U"GameClear");
	}
	if (player.died()) {
		changeScene(U"GameClear");
	}
}

void Stage1::draw() const
{
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
					chipIndex == 2) // 0 の場合は描画しない
				{
					mapchip.get(chipIndex).draw(pos);
				}
			}
		}

		// 敵キャラクターの描画
		for (auto& enemy : enemies) enemy->draw();

		// オブジェクトの描画
		{
			for (const auto& stair : stairs)  stair->draw();
		}

		// ゲームクリア領域
		gameClearBody.draw(Color{ 255, 255, 0 });

		{
			//歩行アニメーションのインデックス(0, 1, 2)
			player.draw();
		}
	}
}
