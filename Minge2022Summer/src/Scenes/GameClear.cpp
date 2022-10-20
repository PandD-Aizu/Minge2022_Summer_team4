# include "GameClear.hpp"

GameClear::GameClear(const InitData& init)
	: IScene{ init }
{
	
}

void GameClear::update()
{
	// ゲームクリアーの表示
	gameClearFont(U"Game Clear").drawAt(Vec2{ Scene::Width() / 2, Scene::Height() / 2 - 50 }, Color{ 0 });

	// 左クリックで
	if (btn.mouseOver() && MouseL.pressed())
	{
		// 次のステージに遷移
		changeScene(U"Title");
	}
}

void GameClear::draw() const
{
	Scene::SetBackground(Color{ 255, 255, 224 });
	bg.draw();
	btn(btnTexture).draw();
}
