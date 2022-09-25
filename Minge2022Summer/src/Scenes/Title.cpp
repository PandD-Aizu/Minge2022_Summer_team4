# include "Title.hpp"



Title::Title(const InitData& init)
	: IScene{ init }
{
	Print << U"Title::Title()";
}

void Title::update()
{
	// 左クリックで
	if (btn.mouseOver() && MouseL.pressed())
	{
		// ゲームシーンに遷移
		changeScene(U"Game");
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });
	bg.draw();

	FontAsset(U"TitleFont")(U"My Game").draw();
	btn(btnTexture).draw();
}

