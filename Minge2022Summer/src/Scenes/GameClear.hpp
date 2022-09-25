#pragma once
# include "../common.hpp"

class GameClear : public App::Scene {
private:
	Font gameClearFont{ 60 };
	const Texture bg{ U"ClearBG.png" };
	const Rect btn{ 64, 280, 512, 64 };
	const Texture btnTexture{ U"backToTitleBtn.png" };
	
public:
	GameClear(const InitData& init);

	void update() override;
	void draw() const override;
};
