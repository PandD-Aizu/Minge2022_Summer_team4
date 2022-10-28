# include "Title.hpp"



Title::Title(const InitData& init)
	: IScene{ init }
{
	Print << U"Title::Title()";
}

void Title::update()
{
	// 左クリックで
	if (btn1.mouseOver() && MouseL.pressed()) {
		// ゲームシーンに遷移
		changeScene(U"Game");
        getData().currentStage = 1;
	}
	if (btn2.mouseOver() && MouseL.pressed()) {
		// ゲームシーンに遷移
        changeScene(U"Game");
        getData().currentStage = 2;
    }
	if (btn3.mouseOver() && MouseL.pressed()) {
		// ゲームシーンに遷移
        changeScene(U"Game");
        getData().currentStage = 3;
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });
	bg.draw();

	FontAsset(U"TitleFont")(U"My Game").draw();
	btn1(btnTexture1).draw();
	btn2(btnTexture2).draw();
	btn3(btnTexture3).draw();
}

