#include "Game.hpp"
# include "../LoadCSV.hpp"

Game::Game(const InitData& init)
: IScene{ init }
{
    for(int32 i=0;i<MAXENEMIESNUM;i++){
        enemiespos[i]={1000,1000};
    }
    stairs << new Stair(Vec2{ 150, 150 }, Vec2{ 250, 150 }, true);
    countswordzombies=0;
    mapLayer0 = LoadCSV(U"layer0.csv");
    mapLayer1 = LoadCSV(U"layer1.csv");
    mapLayer2 = LoadCSV(U"layer2.csv");
    for (int32 y = 0; y < MapSize.y; ++y)
    {
        for (int32 x = 0; x < MapSize.x; ++x)
        {
            const Point pos{ (x * MapChip::MapChipSize), (y * MapChip::MapChipSize) };
            if (mapLayer2[y][x] == 5)
            {
                swordzombie[countswordzombies].getmypos(pos);
                countswordzombies++;
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
}


void Game::update()
{
	// オブジェクトの状態更新
	{
		for (const auto& stair : stairs)  stair->update(&player.pos);
	}

	camera.update();
	camera.setCenter(player.pos);

    //camera.update();
    //const auto t = camera.createTransformer();
    for(int32 i=0;i<countswordzombies;i++){
        enemiespos[i]=swordzombie[i].sendmypos();
    }
    player.getenemiespos(enemiespos);
    player.update();
    //////////////////////
    // プレイヤーの移動
    //////////////////////

	//プレイヤーの方向の変更
	player.decideDirection();
    


	//移動制限処理
    player.moveRestriction(mapLayer1);
	//プレイヤーの移動
    player.moveNextPosition();

    for(int32 i=0;i<countswordzombies;i++){
        swordzombie[i].moveRestriction(mapLayer1);
        swordzombie[i].moveNextPosition();
        swordzombie[i].getplayerpos(player.pos);
        swordzombie[i].update();
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
    if(player.died()){
        changeScene(U"GameClear");
    }
}

void Game::draw() const
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
                    chipIndex != 0) // 0 の場合は描画しない
                {
                    mapchip.get(chipIndex).draw(pos);
                }
            }
        }
        for(int32 i=0;i<countswordzombies;i++){
            swordzombie[i].draw();
        }

		// オブジェクトの描画
		{
			for (const auto& stair : stairs)  stair->draw();
        }

		// ゲームクリア領域
		gameClearBody.draw(Color{ 255, 255, 0 });

        {
			//歩行アニメーションのインデックス(0, 1, 2)
			player.drawWalk();
        }
    }
}
