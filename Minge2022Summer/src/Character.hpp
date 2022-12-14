#pragma once
# include "common.hpp"

/*
* 使い方
* プレイヤー以外のキャラクターを作成する場合は移動関数は使用しない方がいいです
* 移動させたい場合は継承して新しい関数を作ってください。
* ドロー関数を作成する際はPlayer.cppとPlayer.hppを参考にして
* 画像の読み込みと関数の作成を行ってください。
*/


class Character {
protected:

	
	Vec2 velocity;
	double walkSpeed = 2;

	// テクスチャ読み込み
	const Texture CharacterTexture;
	
	// テクスチャのサイズ
	const Vec2 textureSize{ 64, 64 };
	// 中心の相対座標（テクスチャの左上からの相対座標
	const Vec2 textureCenter{ 32, 44 };
	

	// 方向: (基準4から 上:-1 下:+1 左:-3 右:+3)
	// 0:↖  1:←  2:↙  3:↑  4:停止  5:↓  6:↗  7:→  8:↘
	int32 direction = 4;
	int32 lastDirection = 7;

	int hp;


	//プレイヤーの移動に使う
	Vec2 nextPos;

	Vec2 knockBackForce{ 0, 0 };

public:
	Vec2 pos;

	// 当たり判定の起点の座標（中心からの相対座標）
	const Vec2 collisionPoint{ -15, -15 };
	// 当たり判定の幅、高さ
	const Vec2 collisionSize{ 30, 30 };

	Character();

	virtual void update();
	//void decideDirection();			//プレイヤーの方向を決める
	void addKnockBack(Vec2 targetPos, double force = 10);
	void applyKnockBack();
	void moveRestriction();		//移動制限
	void groundMapChipCollision();        //マップチップとの当たり判定処理
	void moveNextPosition();		//プレイヤーの移動

	virtual void draw() const;

};

