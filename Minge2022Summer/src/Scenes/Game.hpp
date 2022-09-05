#pragma once
# include "../common.hpp"
# include "../MapChip.hpp"


class Game : public App::Scene {
private:
	// マップチップ用テクスチャ
	MapChip mapchip;

	Camera2D camera;
	RenderTexture renderTexture;

	// マップのセルの数（横 20, 縦 15 マス）
	Size MapSize{20, 15};
	Grid<int> mapLayer0;
	Grid<int> mapLayer1;
	Vec2 currentPosition;
	Vec2 currentVelocity;
	double walkSpeed = 2.5;

	// 方向: (下: 0, 左: 1, 右: 2, 上: 3) 
	int32 direction = 0;

	Texture PlayerTexture;

public:
	Game(const InitData& init);

	void update() override;
	void draw() const override;
};
