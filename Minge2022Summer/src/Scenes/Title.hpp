# include "../common.hpp"

class Title : public App::Scene
{
	const Texture bg{ U"TitleBG.png" };
	const Rect btn1{ 212, 280, 216, 72 };
	const Rect btn2{ 92, 370, 216, 72 };
	const Rect btn3{ 332, 370, 216, 72 };

	const Texture btnTexture1{ U"startBtn.png" };
	const Texture btnTexture2{ U"startBtn.png" };
	const Texture btnTexture3{ U"startBtn.png" };
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;
};
