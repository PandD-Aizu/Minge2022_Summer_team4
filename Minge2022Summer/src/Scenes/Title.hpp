# include "../common.hpp"

class Title : public App::Scene
{
	const Texture bg{ U"TitleBG.png" };
	const Rect btn_stage{ Arg::center(Scene::Width() / 2 + 50, Scene::Height() / 2 + 100), 250, 100 };
	const Rect btn_left{ Arg::center(Scene::Width() / 2 + 50 - 200, Scene::Height() / 2 + 100), 50, 50 };
	const Rect btn_right{ Arg::center(Scene::Width() / 2 + 50 + 200, Scene::Height() / 2 + 100), 50, 50};

	Texture texture_stage{ U"stage1.png" };
	const Texture texture_left{ U"left.png" };
	const Texture texture_right{ U"right.png" };
	int32 selectedStage = 1;
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;
};
