#pragma once
# include "../../common.hpp"

class Object {
protected:
	const Texture objTexture{U"Sprites/weapons.png"};
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
	const Texture explosionTexture{ U"Sprites/explosion.png" };

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
	Timer homingTimer;

public:

	HomingBullet(Vec2 _pos, double startDeg = 30_deg, double _speed = 0.3, double force = 0.03, SecondsF homingTime = -1s);

	void update() override;
	void draw() const override;
	void accToPlayer(Vec2 *);
};

class BounceBullet : public Object {
private:
	double speed;
	double force;
	double direction;
	int32 bounceLimit;
	int bounceAnimT = 0;

public:

	BounceBullet(Vec2 _pos, double startDeg = 30_deg, double _speed = 0.3, double force = 0.03, int32 _bounceLimit = 1);

	void update() override;
	void draw() const override;
};

class Arrow : public Object {
private:
	double speed;
	double direction;

public:
	Quad body;

	Arrow(Vec2 _pos, double startDeg = 30_deg, double _speed = 0.3);

	void update() override;
	void draw() const override;
};

class AccelerationFloor : public Object {
private:
	double speed;
	double direction;
	double force;

public:
	Quad body;

	AccelerationFloor(Vec2 _pos, double _direction, double force);

	void update() override;
	void draw() const override;

	Vec2 getAcceleration();
};
