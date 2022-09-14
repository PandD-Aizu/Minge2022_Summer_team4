#pragma once
# include "common.hpp"
# include "MapChip.hpp"

class Character {
private:

	Vec2 playerPosition;
	Vec2 playerVelocity;
	double playerWalkSpeed = 1;


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

	// 方向: (基準4から 上:-1 下:+1 左:-3 右:+3)
	// 0:↖  1:←  2:↙  3:↑  4:停止  5:↓  6:↗  7:→  8:↘
	int32 playerDirection = 4;
	int32 lastPlayerDirection = 7;

	


	//プレイヤーの移動に使う
	Vec2 nextPos;

	

public:
	Character();

	void characterDirection();			//プレイヤーの方向を決める
	void movementRestriction(Grid<int>);		//移動制限
	void moveNextPosition();		//プレイヤーの移動

	void drawWalk() const;		//プレイヤーの歩行の描画
	
};
