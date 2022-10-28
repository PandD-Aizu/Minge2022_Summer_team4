#pragma once
# include "../common.hpp"

class GameOver : public App::Scene {
private:
	Font gameClearFont{ 60 };
	const Texture bg{ U"GameOverBG.png" };
	const Rect continueBtn{ 64, 280, 512, 64 };
	const Texture continueBtnTexture{ U"backToTitleBtn.png" };
	const Rect titleBtn{ 64, 376, 512, 64 };
	const Texture titleBtnTexture{ U"backToTitleBtn.png" };

public:
	GameOver(const InitData& init);

	void update() override;
	void draw() const override;
};
