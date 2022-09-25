#pragma once
# include "common.hpp"
# include "MapChip.hpp"

/*
* 使い方
* プレイヤー以外のキャラクターを作成する場合は移動関数は使用しない方がいいです
* 移動させたい場合は継承して新しい関数を作ってください。
* ドロー関数を作成する際はPlayer.cppとPlayer.hppを参考にして
* 画像の読み込みと関数の作成を行ってください。
*/


class Character {
protected:

	
	Vec2 characterVelocity;
	double characterWalkSpeed = 1;

	// テクスチャ読み込み
	/*const Texture CharacterTexture{ U"playerSprite.png" };*/
	
	// テクスチャのサイズ
	const Vec2 characterTextureSize{ 20, 28 };
	// 中心の相対座標（テクスチャの左上からの相対座標
	const Vec2 characterTextureCenter{ 10, 20 };
	

	// 方向: (基準4から 上:-1 下:+1 左:-3 右:+3)
	// 0:↖  1:←  2:↙  3:↑  4:停止  5:↓  6:↗  7:→  8:↘
	int32 characterDirection = 4;
	int32 lastCharacterDirection = 7;

	


	//プレイヤーの移動に使う
	Vec2 nextPos;

	

public:
	Vec2 characterPosition;

	// 当たり判定の起点の座標（中心からの相対座標）
	const Vec2 characterCollisionPoint{ -8, -8 };
	// 当たり判定の幅、高さ
	const Vec2 characterCollisionSize{ 16, 16 };

	Character();

	void decideCharacterDirection();			//プレイヤーの方向を決める
	void moveRestriction(Grid<int>);		//移動制限
	void moveNextPosition();		//プレイヤーの移動

};

