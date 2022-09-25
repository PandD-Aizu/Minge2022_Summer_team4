#pragma once
# include "../common.hpp"
# include "../MapChip.hpp"
# include "../Character.hpp"
# include "myCamera.hpp"
# include "../Player.hpp"
# include "Enemies/SwordZombie.hpp"

#define MAXSWORDZOMBIESNUM 10



class Game : public App::Scene {
private:
	MyCamera camera;
	// マップチップ用テクスチャ
	MapChip mapchip;

	RenderTexture renderTexture;

    SwordZombie swordzombie[MAXSWORDZOMBIESNUM];
    int32 countswordzombies;
	// マップのセルの数（横 20, 縦 15 マス）
	Size MapSize{20, 15};
	Grid<int> mapLayer0;
    Grid<int> mapLayer1;
    Grid<int> mapLayer2;

    
    ////////////////////////
    // プレイヤーの初期化
    ////////////////////////


	Player player;


    
	// ゲームクリア領域
	Circle gameClearBody{ Vec2{ 16 * 16 + 8, 16 * 4 + 8}, 8 };


public:
	Game(const InitData& init);

	void update() override;
	void draw() const override;
};
