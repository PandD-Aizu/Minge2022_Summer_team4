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

	////////////////////////
	// プレイヤーの初期化
	////////////////////////
	
	// テクスチャ読み込み
	const Texture PlayerTexture{ U"playerSprite.png" };
	// テクスチャのサイズ
	const Vec2 playerTextureSize{ 20, 28 };
	// 中心の相対座標（テクスチャの左上からの相対座標
	const Vec2 playerTextureCenter{ 10, 20 };
	// 当たり判定の起点の座標（中心からの相対座標）
	const Vec2 playerCollisionPoint{ -7, -7 };
	// 当たり判定の幅、高さ
	const Vec2 playerCollisionSize{ 14, 14 };

	// 現在の座標
    
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
    Circle playerCircle = Circle(1000, 1000, 0);

	Vec2 playerPosition{ 6 * 16, 6 * 16 };

	// 現在の移動速度
	Vec2 playerVelocity{ 0, 0 };
	// 移動速度
	constexpr double playerWalkSpeed = 1;
	// 現在向いている方向: (下: 0, 左: 1, 右: 2, 上: 3, 左下: 4, 左上: 5, 右下: 6, 右上: 7)
	int32 playerDirection = 0;


	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	RenderTexture renderTexture{ 320, 240 };

	//////////////////
	// メインループ
	//////////////////
	while (System::Update()) {
		//camera.update();
		//const auto t = camera.createTransformer();

		//////////////////////
		// プレイヤーの移動
		//////////////////////

		// 速度を0で初期化する
		playerVelocity = Vec2(0, 0);
		// プレイヤー移動中フラグ
		bool isPlayerMoving(false);
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

		// 移動制限処理
		{
			// x方向の移動制限
			Vec2 nextPos = playerPosition + playerVelocity*Vec2(1, 0);
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
        
        // 対応した階段の1マス右にワープする
        for(int32 i = 0; i < STAIRS; i++){
            if(playerCircle.intersects(upstairRects[i])){
                downstairposes[i] = { downstairposes[i].x + 3 * MapChip::MapChipSize / 2, downstairposes[i].y + MapChip::MapChipSize / 2 + 1 };
                playerPosition=downstairposes[i];
            }else if(playerCircle.intersects(downstairRects[i])){
                upstairposes[i] = { upstairposes[i].x + 3 * MapChip::MapChipSize / 2, upstairposes[i].y + MapChip::MapChipSize / 2 + 1 };
                playerPosition=upstairposes[i];
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
                playerCircle = Circle{ playerPosition.x, playerPosition.y, 1 };
                playerCircle.draw();
				
				int32 animationIndex;
				if (isPlayerMoving) animationIndex = static_cast<int32>(Scene::Time() * 10 / 2) % 3;
				else animationIndex = 0;

				PlayerTexture((playerTextureSize.x * animationIndex), (playerTextureSize.y * playerDirection), playerTextureSize.x, playerTextureSize.y)
					.draw(
						static_cast<int32>(playerPosition.x - playerTextureCenter.x),
						static_cast<int32>(playerPosition.y - playerTextureCenter.y)
					);
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
