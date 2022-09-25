# include "Title.hpp"



Title::Title(const InitData& init)
	: IScene{ init }
{
	Print << U"Title::Title()";
}

void Title::update()
{
   
        

        
	// 左クリックで
	if (SimpleGUI::Button(U"スタート", Vec2{ 160, 120 }, 320))
	{
		// ゲームシーンに遷移
		changeScene(U"Game");
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });
	bg.draw();

	FontAsset(U"TitleFont")(U"My Game").drawAt(400, 100);

	Circle{ Cursor::Pos(), 50 }.draw(Palette::Orange);
}

