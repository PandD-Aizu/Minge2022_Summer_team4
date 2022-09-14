#pragma once
# include "common.hpp"
# include "MapChip.hpp"

class Player {
private:

	Vec2 playerPosition;
	Vec2 playerVelocity;
	double playerWalkSpeed = 1;


	// テクスチャ読み込み
	const Texture PlayerTexture{ U"playerSprite2.png" };
	// テクスチャのサイズ
	const Vec2 playerTextureSize{ 20, 28 };
	// 中心の相対座標（テクスチャの左上からの相対座標
	const Vec2 playerTextureCenter{ 10, 20 };
	// 当たり判定の起点の座標（中心からの相対座標）
	const Vec2 playerCollisionPoint{ -8, -8 };
	// 当たり判定の幅、高さ
	const Vec2 playerCollisionSize{ 16, 16 };

	// 方向: (下: 0, 左: 1, 右: 2, 上: 3) 
	int32 playerDirection = 0;

	// プレイヤー移動中フラグ
	bool isPlayerMoving;


	//プレイヤーの移動に使う
	Vec2 nextPos;

	

public:
	Player();

	void checkPlayerMove();			//プレイヤーの方向を決める
	void movementRestriction(Grid<int>);		//移動制限
	void moveNextPosition();		//プレイヤーの移動

	void drawWalk() const;		//プレイヤーの歩行の描画
	
};
