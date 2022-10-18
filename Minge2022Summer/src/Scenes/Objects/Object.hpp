#pragma once
# include "../../common.hpp"

class Object {
public:
	// オブジェクト識別
	int id;
	// アニメーションの枚数
	int animNum;
	int animIndex;

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
