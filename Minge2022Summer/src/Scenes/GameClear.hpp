#pragma once
# include "../common.hpp"

class GameClear : public App::Scene {
private:
	Font gameClearFont{ 60 };
public:
	GameClear(const InitData& init);

	void update() override;
	void draw() const override;
};


class GameOver : public App::Scene {
private:
	Font gameClearFont{ 60 };
public:
	GameOver(const InitData& init);

	void update() override;
	void draw() const override;
};
