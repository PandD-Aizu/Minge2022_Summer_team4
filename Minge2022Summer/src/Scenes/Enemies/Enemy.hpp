# pragma once
#include"../../Character.hpp"
#include "../Objects/Object.hpp"

class Enemy : public Character{
private:
public:
	Vec2 playerPos;
	void getPlayerPos(Vec2);
	virtual void emitObject(Array<Object*>*) {}
};
