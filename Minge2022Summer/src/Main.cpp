//# include <Siv3D.hpp>
# include "common.hpp"
# include "Scenes/Stage1.hpp"
# include "Scenes/Title.hpp"
# include "Scenes/GameClear.hpp"

Grid<int> mapLayer0;
Grid<int> mapLayer1;

void Main()
{

	// ウィンドウを 640x480 にリサイズする
	//Scene::SetResizeMode(ResizeMode::Keep);
	Window::Resize(640, 480);
	Scene::SetTextureFilter(TextureFilter::Nearest);

	//FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);

	// シーンマネージャーを作成
	App manager;

	// タイトルシーン（名前は "Title"）を登録
	manager.add<Title>(U"Title");

	// ゲームシーン（名前は "Stage"）を登録
	manager.add<Stage1>(U"Stage1");

	// ゲームクリアーシーン（名前は "GameClear"）を登録
	manager.add<GameClear>(U"GameClear");

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
