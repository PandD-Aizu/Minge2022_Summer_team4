# include "../common.hpp"

class Title : public App::Scene
{
	const Texture bg{ U"TitleBG.png" };
	const Rect btn{ 212, 280, 216, 72 };
	const Texture btnTexture{ U"startBtn.png" };
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;
};
