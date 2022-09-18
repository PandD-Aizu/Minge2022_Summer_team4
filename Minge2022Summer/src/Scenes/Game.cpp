#include "Game.hpp"
# include "../LoadCSV.hpp"

Game::Game(const InitData& init)
: IScene{ init }
{
    mapLayer0 = LoadCSV(U"layer0.csv");
    mapLayer1 = LoadCSV(U"layer1.csv");
    
    if ((mapLayer0.size() != MapSize) || (mapLayer1.size() != MapSize)) {
        // MapSize と、ロードしたデータのサイズが一致しない場合のエラー
        throw Error{ U"mapLayer0: {}, mapLayer1: {}"_fmt(mapLayer0.size(), mapLayer1.size()) };
    }
    
    //// 現在の座標
    //playerPosition = Vec2{ 6 * 16, 6 * 16 };
    //// 現在の移動速度
    //playerVelocity = Vec2{ 0, 0 };
    
    // マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
    renderTexture = RenderTexture{ 320, 240 };

	// カメラの位置と大きさを初期化
	camera.setScreen(Rect(Scene::Size()));
	camera.setScale(2);
	camera.setTargetScale(2);
	camera.setCenter(player.characterPosition);
}


void Game::update()
{

	camera.update();
	camera.setCenter(player.characterPosition);

    //camera.update();
    //const auto t = camera.createTransformer();
    
    //////////////////////
    // プレイヤーの移動
    //////////////////////

	//プレイヤーの方向の変更
	player.decideCharacterDirection();
    


	//移動制限処理
	player.moveRestriction(mapLayer1);
	player.groundMapChipCollision(mapLayer0);

	//プレイヤーの移動
	player.moveNextPosition();
	

    
	// ゲームクリア領域の当たり判定
	if (gameClearBody.intersects(Rect{
		static_cast<int32>(player.characterPosition.x + player.characterCollisionPoint.x),
		static_cast<int32>(player.characterPosition.y + player.characterCollisionPoint.y),
		static_cast<int32>(player.characterCollisionSize.x),
		static_cast<int32>(player.characterCollisionSize.y)
		}))
	{
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

		// ゲームクリア領域
		gameClearBody.draw(Color{ 255, 255, 0 });

        {


			//歩行アニメーションのインデックス(0, 1, 2)
			player.drawWalk();



            
        }
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

