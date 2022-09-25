# include "GameClear.hpp"

GameClear::GameClear(const InitData& init)
	: IScene{ init }
{
	
}

void GameClear::update()
{
	// ゲームクリアーの表示
	gameClearFont(U"Game Clear").drawAt(Vec2{ Scene::Width() / 2, Scene::Height() / 2 - 50 }, Color{ 0 });

	// 次のステージへ進むボタン
	if (SimpleGUI::ButtonAt(U"次のステージ", Vec2{ Scene::Width() / 2, Scene::Height() / 2 + 50 }))
	{
		// 次のステージへ進む処理
		changeScene(U"Game");
	}
}

void GameClear::draw() const
{
	Scene::SetBackground(Color{ 255, 255, 224 });
	bg.draw();
}
