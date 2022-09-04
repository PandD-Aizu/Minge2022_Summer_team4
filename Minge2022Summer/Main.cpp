# include <Siv3D.hpp>
# include "MapChip.hpp"
# include "LoadCSV.hpp"

void Main()
{
	// ウィンドウを 640x480 にリサイズする
	//Scene::SetResizeMode(ResizeMode::Keep);
	Window::Resize(640, 480);
	Scene::SetTextureFilter(TextureFilter::Nearest);

	// マップチップ用テクスチャ
	MapChip mapchip;

	Camera2D camera;

	// マップのセルの数（横 20, 縦 15 マス）
	constexpr Size MapSize{ 20, 15 };

	Grid<int32> mapLayer0 = LoadCSV(U"layer0.csv");
	Grid<int32> mapLayer1 = LoadCSV(U"layer1.csv");

	if ((mapLayer0.size() != MapSize) || (mapLayer1.size() != MapSize)) {
		// MapSize と、ロードしたデータのサイズが一致しない場合のエラー
		throw Error{ U"mapLayer0: {}, mapLayer1: {}"_fmt(mapLayer0.size(), mapLayer1.size()) };
	}

	const Texture PlayerTexture{ U"playerSprite.png" };

	// 現在の座標
	Vec2 currentPosition{ 6 * 16, 6 * 16 };
    
    // 階段の場所を保存しておく配列
    Point upstairposes[STAIRS];
    Point downstairposes[STAIRS];
    
    // 階段の当たり判定の位置を保存しておく配列とその初期化
    Rect upstairRects[STAIRS];
    Rect downstairRects[STAIRS];
    for(int32 i = 0; i < STAIRS; i++){
        upstairRects[i] = Rect(0, 0, 0, 0);
        downstairRects[i] = Rect(0, 0, 0, 0);
    }
    
    // プレイヤーの当たり判定の位置を初期化しておく 階段の当たり判定と違う位置に当たり判定を初期化する必要があるため、1200, 1200
    Circle playerCircle = Circle(1200, 1200, 0);

	// 現在の移動速度
	Vec2 currentVelocity{ 0, 0 };

	constexpr double walkSpeed = 2.5;

	// 方向: (下: 0, 左: 1, 右: 2, 上: 3) 
	int32 direction = 0;

	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	RenderTexture renderTexture{ 320, 240 };

	while (System::Update()) {
		//camera.update();
		//const auto t = camera.createTransformer();

		// 移動先のセル座標を設定
		currentVelocity = Vec2(0, 0);
		if (KeyDown.pressed()) { // ↓ キー	
			currentVelocity.y = walkSpeed;
			direction = 0;
		}
		if (KeyUp.pressed()) { // ↑ キー
			currentVelocity.y = -walkSpeed;
			direction = 3;
		}
		if (KeyLeft.pressed()) { // ← キー
			currentVelocity.x = -walkSpeed;
			direction = 1;
		}
		if (KeyRight.pressed()) { // → キー
			currentVelocity.x = walkSpeed;
			direction = 2;
		}

		// 移動制限処理
		{
			Vec2 nextPos = currentPosition + currentVelocity*Vec2(1, 0);
			Point nextCell(static_cast<int32>(nextPos.x / MapChip::MapChipSize), static_cast<int32>(nextPos.y / MapChip::MapChipSize));
			//Print << nextPos << nextCell;
			//Print << mapLayer1;

			if (mapLayer1[nextCell.y][nextCell.x] != 0) {
				if (currentVelocity.x > 0) {
					nextPos.x = nextCell.x * MapChip::MapChipSize - 1;
				}
				else {
					nextPos.x = (nextCell.x + 1) * MapChip::MapChipSize;
				}
			}

			nextPos += currentVelocity * Vec2(0, 1);
			nextCell = Point(static_cast<int32>(nextPos.x / MapChip::MapChipSize), static_cast<int32>(nextPos.y / MapChip::MapChipSize));

			if (mapLayer1[nextCell.y][nextCell.x] != 0) {
				if (currentVelocity.y > 0) {
					nextPos.y = nextCell.y * MapChip::MapChipSize - 1;
				}
				else {
					nextPos.y = (nextCell.y + 1) * MapChip::MapChipSize;
				}
			}
			currentPosition = nextPos;
		}
        
        // 対応した階段の1マス右にワープする
        for(int32 i = 0; i < STAIRS; i++){
            if(playerCircle.intersects(upstairRects[i])){
                downstairposes[i] = { downstairposes[i].x + 3 * MapChip::MapChipSize / 2, downstairposes[i].y + MapChip::MapChipSize / 2 + 1 };
                currentPosition=downstairposes[i];
            }else if(playerCircle.intersects(downstairRects[i])){
                upstairposes[i] = { upstairposes[i].x + 3 * MapChip::MapChipSize / 2, upstairposes[i].y + MapChip::MapChipSize / 2 + 1 };
                currentPosition=upstairposes[i];
            }
        }

		////////////////////////////////
		//
		//	描画
		//
		////////////////////////////////

		{
			// renderTexture を描画先として設定
			const ScopedRenderTarget2D rt{ renderTexture };

			// スペースキーを押すと通行可能判定を可視化する
			const bool showPassable = KeySpace.pressed();

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
                        // 階段がある場所を保持しておく
                        if(chipIndex / STAIRS == 3){
                            upstairposes[chipIndex % STAIRS] = pos;
                        }else if(chipIndex / STAIRS == 4){
                            downstairposes[chipIndex % STAIRS] = pos;
                        }
                        
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
            
            // それぞれの階段に別々の当たり判定を配置
            for(int32 i = 0; i < STAIRS; i++){
                upstairRects[i] = Rect(upstairposes[i], MapChip::MapChipSize);
                downstairRects[i] = Rect(downstairposes[i], MapChip::MapChipSize);
            }
            
			{
				// 歩行のアニメーションのインデックス (0, 1, 2)
				int32 animationIndex = 1;

				PlayerTexture((20 * animationIndex), (28 * direction), 20, 28).draw(currentPosition.x - 10, currentPosition.y - 21);
                playerCircle = Circle{ currentPosition.x, currentPosition.y, 1 };
                playerCircle.draw();
			}
		}

		{
			// テクスチャ拡大描画時にフィルタリング（なめらかなな拡大処理）をしないサンプラーステートを適用
			const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

			// renderTexture を 2 倍のサイズでシーンに描画
			renderTexture.scaled(2).draw();
		}
		{
			// 中心とズームアップ倍率の目標値をセットして、時間をかけて変更する
			//camera.setTargetCenter(Vec2{ Scene::Width()/2, Scene::Height()/2});
			//camera.setTargetScale(1.0);
		}
		//camera.draw(Palette::Orange);
	}
}
