# include "../common.hpp"

class Title : public App::Scene
{
public:

	Title(const InitData& init)
		: IScene{ init }
	{
		Print << U"Title::Title()";
	}

	~Title()
	{
		Print << U"Title::~Title()";
	}

	void update() override
	{
		// 左クリックで
		if (MouseL.down())
		{
			// ゲームシーンに遷移
			changeScene(U"Game");
		}
	}

	void draw() const override
	{
		Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });

		FontAsset(U"TitleFont")(U"My Game").drawAt(400, 100);

		Circle{ Cursor::Pos(), 50 }.draw(Palette::Orange);
	}
};
