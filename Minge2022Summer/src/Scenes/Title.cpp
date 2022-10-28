# include "Title.hpp"



Title::Title(const InitData& init)
	: IScene{ init }
{
	Print << U"Title::Title()";
}

void Title::update()
{
	// 左クリックで
	if (btn_left.mouseOver() && MouseL.down()) {
		if (selectedStage > 1) {
			selectedStage--;
			texture_stage = Texture{ U"stage{}.png"_fmt(selectedStage) };
		}
	}
	if (btn_right.mouseOver() && MouseL.down()) {
		if (selectedStage < 7) {
			selectedStage++;
			texture_stage = Texture{ U"stage{}.png"_fmt(selectedStage) };
		}
	}

	if (btn_stage.mouseOver() && MouseL.down()) {
		getData().currentStage = selectedStage;
		changeScene(U"Game");
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.3, 0.4, 0.5 });
	bg.draw();

	FontAsset(U"TitleFont")(U"My Game").draw();
	btn_stage(texture_stage).draw();
	btn_left(texture_left).draw();
	btn_right(texture_right).draw();
}

