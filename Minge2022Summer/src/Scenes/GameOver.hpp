#pragma once
# include "../common.hpp"

class GameOver : public App::Scene {
private:
	Font gameClearFont{ 60 };
	const Texture bg{ U"GameOverBG.png" };
	const Rect continueBtn{ Arg::center(Scene::Width() / 2, Scene::Height() / 2 + 60), 224, 64 };
	const Texture continueBtnTexture{ U"GameOver_restart.png" };
	const Rect titleBtn{ Arg::center(Scene::Width() / 2, Scene::Height() / 2 + 150), 368, 64 };
	const Texture titleBtnTexture{ U"GameOver_backToTitleBtn.png" };

public:
	GameOver(const InitData& init);

	void update() override;
	void draw() const override;
};
