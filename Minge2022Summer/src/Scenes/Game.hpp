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
	Vec2 playerPosition;
	Vec2 playerVelocity;
	double playerWalkSpeed = 1;
    
    ////////////////////////
    // プレイヤーの初期化
    ////////////////////////

    // テクスチャ読み込み
    const Texture PlayerTexture{ U"playerSprite.png" };
    // テクスチャのサイズ
    const Vec2 playerTextureSize{ 20, 28 };
    // 中心の相対座標（テクスチャの左上からの相対座標
    const Vec2 playerTextureCenter{ 10, 20 };
    // 当たり判定の起点の座標（中心からの相対座標）
    const Vec2 playerCollisionPoint{ -8, -8 };
    // 当たり判定の幅、高さ
    const Vec2 playerCollisionSize{ 16, 16 };

	// 方向: (基準4 上:-1 下:+1 左:-3 右:+3) 
	int32 playerDirection = 4;
	int32 lastPlayerDirection = 7;
    
	// ゲームクリア領域
	Circle gameClearBody{ Vec2{ 16 * 16 + 8, 16 * 4 + 8}, 8 };

public:
	Game(const InitData& init);

	void update() override;
	void draw() const override;
};
