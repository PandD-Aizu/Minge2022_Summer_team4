# include "../common.hpp"

class Title : public App::Scene
{
	const Texture bg{ U"TitleBG.png" };
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;
};
