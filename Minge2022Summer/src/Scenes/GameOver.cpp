# include "GameOver.hpp"

GameOver::GameOver(const InitData& init)
	: IScene{ init }
{

}

void GameOver::update()
{
	// ゲームクリアーの表示
	gameClearFont(U"Game Over").drawAt(Vec2{ Scene::Width() / 2, Scene::Height() / 2 - 50 }, Color{ 255 });

	// 左クリックで
	if (continueBtn.mouseOver() && MouseL.pressed())
	{
		// コンティニュー
		changeScene(U"Game");
	}
	if (titleBtn.mouseOver() && MouseL.pressed())
	{
		// 次のステージに遷移
		changeScene(U"Title");
	}
}

void GameOver::draw() const
{
	Scene::SetBackground(Color{ 20, 20, 20 });
	bg.draw();
	continueBtn(continueBtnTexture).draw();
	titleBtn(titleBtnTexture).draw();
}
