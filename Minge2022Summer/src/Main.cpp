//# include <Siv3D.hpp>
# include "common.hpp"
# include "Scenes/Game.hpp"
# include "Scenes/Title.hpp"

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

	// ゲームシーン（名前は "Game"）を登録
	manager.add<Game>(U"Game");

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
