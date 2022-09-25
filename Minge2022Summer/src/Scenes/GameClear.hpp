#pragma once
# include "../common.hpp"

class GameClear : public App::Scene {
private:
	Font gameClearFont{ 60 };
	const Texture bg{ U"ClearBG.png" };

public:
	GameClear(const InitData& init);

	void update() override;
	void draw() const override;
};
