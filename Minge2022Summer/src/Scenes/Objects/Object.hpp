#pragma once
# include "../../common.hpp"

class Object {
protected:
public:
	// オブジェクト識別
	int id;
	// アニメーションの枚数
	int animNum;
	int animIndex;
	Vec2 pos;

	bool destructorFlag = false; // 削除フラグ

	virtual void update() {}
	virtual void draw() const {}
	Object() {}
	Object(Vec2 pos) :pos(pos) {}
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
	int32 state = 0; // 現在の状態（0: 爆発待機 1: 爆発中）
	int32 range; // 爆発範囲

	Bomb(Vec2 pos, SecondsF limit = 3s, int32 range = 16);

	void update() override;

	void draw() const override;
};

class HomingBullet : public Object {
private:
	double speed;
	double force;
	double direction;

public:

	HomingBullet(Vec2 _pos, double _speed = 0.1, double force = 0.01, double startDeg = 30_deg);

	void update() override;
	void draw() const override;
	void accToPlayer(Vec2 *);
};
