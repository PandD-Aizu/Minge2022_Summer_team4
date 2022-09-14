#pragma once
# include "../common.hpp"
# include "../MapChip.hpp"
# include "../Character.hpp"


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

    
    ////////////////////////
    // プレイヤーの初期化
    ////////////////////////

	Character player;

public:
	Game(const InitData& init);

	void update() override;
	void draw() const override;
};
