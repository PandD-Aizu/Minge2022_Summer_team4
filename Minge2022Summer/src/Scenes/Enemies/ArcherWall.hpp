#pragma once
#include"Enemy.hpp"

class ArcherWall : public Enemy {
private:
	double speed;
	int shotCnt = 100;
	int maxShotCnt;
public:
	ArcherWall(Point, int, int);
	void update() override;
	void draw()const override;
	void emitObject(Array<Object*>*) override;
};
