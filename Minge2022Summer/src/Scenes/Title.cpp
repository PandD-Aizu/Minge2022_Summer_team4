# include "Title.hpp"



Title::Title(const InitData& init)
	: IScene{ init }
{
	Print << U"Title::Title()";
}

void Title::update()
{
   
        

        
	// 左クリックで
	if (SimpleGUI::Button(U"ステージ1", Vec2{ 160, 120 }, 320))
	{
		// ゲームシーンに遷移
		changeScene(U"Game");
	}
    if (SimpleGUI::Button(U"ステージ2", Vec2{ 160, 220 }, 320))
    {
        // ゲームシーンに遷移
        //changeScene(U"Game2");
   }
    if (SimpleGUI::Button(U"ステージ3", Vec2{ 160, 320 }, 320))
    {
        // ゲームシーンに遷移
        //changeScene(U"Game3");
   }
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });

	FontAsset(U"TitleFont")(U"My Game").drawAt(400, 100);

	Circle{ Cursor::Pos(), 50 }.draw(Palette::Orange);
}

