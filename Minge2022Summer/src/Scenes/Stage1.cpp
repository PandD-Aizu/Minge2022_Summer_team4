#include "Stage1.hpp"
//# include "../LoadCSV.hpp"


Stage1::Stage1(const InitData& init)
	: IScene{ init }
{
	AudioAsset(U"mainBGM").setVolume(0.2);
	AudioAsset(U"mainBGM").play();
	countswordzombies = 0;
	mapLayer0 = LoadCSV(U"maps/stage{}/layer0.csv"_fmt(getData().currentStage));
	mapLayer1 = LoadCSV(U"maps/stage{}/layer1.csv"_fmt(getData().currentStage));

	HashTable<int32, Point> stairPair;
	HashTable<int32, Point> stairPairNonRev;

	// layer1上の敵を読み込む
	for (int32 y = 0; y < MapSize.y; ++y)
	{
		for (int32 x = 0; x < MapSize.x; ++x)
		{
			const int32 value = mapLayer1[y][x];
			const Point pos{ (x * MapChip::MapChipSize), (y * MapChip::MapChipSize) };
			switch(value)
			{
			case 5:
				enemies << new SwordZombie(pos);
				break;
			case 6:
				enemies << new Bomber(pos);
				break;
			case 7:
				enemies << new HomingGunner(pos, 500);
				break;
			case 8:
				enemies << new BounceGunner(pos, 500);
				break;
			case 100:
				player.pos = pos;
				break;
			case 101:
				gameClearBody.x = pos.x;
				gameClearBody.y = pos.y;
				break;
			}
			if (value / 10 == 4) {
				enemies << new ArcherWall(pos, 500, value % 10);
			}
			// 5X: 自由に行き来可能な階段
			if (value / 10 == 5) {
				if (stairPair.contains(value % 10)) {
					objects << new Stair(stairPair[value % 10], pos, true);
				}
				else {
					stairPair.emplace(value % 10, pos);
				}
			}
			// 6X: 一方通行の階段（入口）
			if (value / 10 == 6) {
				if (stairPairNonRev.contains(value % 10)) {
					objects << new Stair(pos, stairPairNonRev[value % 10], false);
				}
				else {
					stairPairNonRev.emplace(value % 10, pos);
				}
			}
			// 7X: 一方通行の階段（出口）
			if (value / 10 == 7) {
				if (stairPairNonRev.contains(value % 10)) {
					objects << new Stair(stairPairNonRev[value % 10], pos, false);
				}
				else {
					stairPairNonRev.emplace(value % 10, pos);
				}
			}

			// 1XXX: 矢を放つ罠
			// Xoo: 向き（スプライトの向きと同じ）
			// oXo: 発射間隔（(X + 1) * 0.5秒)
			// ooX: 最初の発射までの遅延（X * 0.5秒）
			if (value / 1000 == 1) {
				const int32 direction = (value % 1000) / 100;
				const int32 duration = ((value % 100) / 10 + 1) * 30;
				const int32 delay = (value % 10) * 30;
				enemies << new ArcherWall(pos, duration, direction, delay);
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
	// =========== デバッグ ==========
	if (KeyB.down()) objects << new Bomb(player.pos); // 敵用の爆弾の設置
	// ===============================

	// オブジェクトの状態更新
	{
		for (auto obj = objects.begin(); obj != objects.end(); obj++) {
			(*obj)->update();

			// 削除フラグ確認
			if ((*obj)->destructorFlag) {
				delete* obj;
				obj = objects.erase(obj);

				if (obj == objects.end()) break; // 削除した結果末尾だったらループを抜ける
			}
		}
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
	if (MouseL.down())player.attack();

	for (auto& enemy : enemies) {
		enemy->getPlayerPos(player.pos);
		if(enemy->isInSenceRange()) enemy->update();
		player.detectEnemyCollision(enemy);
		enemy->emitObject(&objects);
	}


	enemies.remove_if([](Enemy* enm) {return enm->isDefeated(); });

	for (auto& obj : objects) {
		obj->update();
		player.detectObjCollision(obj);
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

		// ゲームクリア領域
		gameClearBody.draw(Color{ 255, 255, 0 });



		// 敵キャラクターの描画
		for (auto& enemy : enemies) if(enemy->pos.y <= player.pos.y)if (enemy->isInSenceRange())  enemy->draw();
		// オブジェクトの描画
		for (const auto& obj : objects) if (obj->pos.y <= player.pos.y) obj->draw();

		{
			//歩行アニメーションのインデックス(0, 1, 2)
			player.draw();
		}

		// 敵キャラクターの描画
		for (auto& enemy : enemies) if (enemy->pos.y > player.pos.y)if (enemy->isInSenceRange())  enemy->draw();
		// オブジェクトの描画
		for (const auto& obj : objects) if (obj->pos.y > player.pos.y) obj->draw();

	}
	player.drawHP();
}
