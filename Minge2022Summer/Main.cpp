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

	// 現在のセル座標
	Point currentCell{ 6, 6 };

	// 移動先のセル座標
	Point nextCell = currentCell;

	constexpr double walkSpeed = 2.5;

	// 歩行の進捗 (移動開始: 0.0, nextCell に到達: 1.0)
	double walkProgress = 1.0;

	// 方向: (下: 0, 左: 1, 右: 2, 上: 3) 
	int32 direction = 0;

	// マップを 320x240 のレンダーテクスチャに描画し、それを最終的に 2 倍サイズで描画する
	RenderTexture renderTexture{ 320, 240 };

	while (System::Update()) {
		//camera.update();
		//const auto t = camera.createTransformer();

		// 現在移動中でない場合、上下左右キーで次に進むセルを変更
		if (currentCell == nextCell) 	{
			//
			// 移動先のセル座標を設定
			//
			if (KeyDown.pressed()) { // ↓ キー	
				++nextCell.y;
				direction = 0;
			}else if (KeyUp.pressed()) { // ↑ キー
				--nextCell.y;
				direction = 3;
			}else if (KeyLeft.pressed()) { // ← キー
				--nextCell.x;
				direction = 1;
			}else if (KeyRight.pressed()) { // → キー
				++nextCell.x;
				direction = 2;
			}

			// nextCell をマップの範囲内に収める
			nextCell.x = Clamp(nextCell.x, 0, (MapSize.x - 1));
			nextCell.y = Clamp(nextCell.y, 0, (MapSize.y - 1));

			// 移動先のセルが通行不能な場合
			if (mapLayer1[nextCell] != 0)
			{
				// nextCell を現在のセル座標にする
				nextCell = currentCell;
			}

			// 移動が発生する場合
			if (currentCell != nextCell)
			{
				// 歩行の進捗を戻す
				walkProgress -= 1.0;
			}
			else
			{
				// 移動しない場合、歩行の進捗は 1.0
				walkProgress = 1.0;
			}
		}

		// 移動中の場合
		if (currentCell != nextCell)
		{
			// 歩行の進捗を進める
			walkProgress += (Scene::DeltaTime() * walkSpeed);

			// 歩行の進捗が 1.0 以上になったら
			if (1.0 <= walkProgress)
			{
				// 現在のセルを nextCell にして移動完了
				currentCell = nextCell;
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

			{
				// 歩行の進捗に基づいて、現在のセル座標を小数で計算
				const Vec2 cellPos = currentCell.lerp(nextCell, walkProgress);

				// 現在のセル座標を描画座標に変換
				const Vec2 pos = (cellPos * MapChip::MapChipSize) + Vec2{ -2, -12 };

				// 歩行のアニメーションのインデックス (0, 1, 2)
				int32 animationIndex = 1;

				// 歩行の進捗が 0.25～0.75 の間は歩行中の絵にする
				if (InRange(walkProgress, 0.25, 0.75))
				{
					// (nextCell.x + nextCell.y) が偶数なら 0, 奇数なら 2
					animationIndex = IsEven(nextCell.x + nextCell.y) ? 0 : 2;
				}

				PlayerTexture((20 * animationIndex), (28 * direction), 20, 28).draw(pos);
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
