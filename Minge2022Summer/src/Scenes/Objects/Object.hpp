#pragma once
# include "../../common.hpp"

class Object {
public:
	// オブジェクト識別
	int id;
	// アニメーションの枚数
	int animNum;
	int animIndex;

	bool destructorFlag = false; // 削除フラグ

	virtual void update() {}
	virtual void draw() const {}
	virtual ~Object() {}
};



class Stair : public Object {
private:
public:
	Vec2 inPos;
	Vec2 outPos;
	bool isReversable;
	bool reverseLock;
	const Texture texture;
	const Vec2 size;

	Stair(Vec2 _inPos, Vec2 _outPos, bool _isReversable);

	void update() override;

	void draw() const override;
};



class Bomb : public Object {
private:
	SecondsF timeLimit;
	Timer timer;

public:
	Vec2 position;
	int32 state = 0; // 現在の状態（0: 爆発待機 1: 爆発中）
	int32 range; // 爆発範囲

	Bomb(Vec2 pos, SecondsF limit = 3s, int32 range = 16);

	void update() override;

	void draw() const override;
};
