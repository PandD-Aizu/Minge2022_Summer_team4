# pragma once
#include"../../Character.hpp"
#include "../Objects/Object.hpp"

class Enemy : public Character{
private:
	const Texture CharacterTexture;

public:
	Vec2 playerPos;
	bool isRestless = false;

	Enemy();
	Enemy(String texture);

	void ensureDirection(); // velocityの値を参照して、directionを決定する
	void draw() const override;

	void getPlayerPos(Vec2);
	bool isDefeated();
	virtual void emitObject(Array<Object*>*) {}
	void isAttacked();
	bool isInSenceRange() const;
};
