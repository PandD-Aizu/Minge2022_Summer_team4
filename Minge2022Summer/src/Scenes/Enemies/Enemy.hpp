# pragma once

#include"../../Character.hpp"
class Enemy : public Character{
private:
public:
	Vec2 playerPos;
	void getPlayerPos(Vec2);
};
