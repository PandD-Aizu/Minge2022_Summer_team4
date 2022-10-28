#pragma once
#include"Enemy.hpp"

class ArcherWall : public Enemy {
private:
	double speed;
	int shotCnt = 100;
	int maxShotCnt;
public:
	ArcherWall(Point mapPos, int maxCnt, int direction, int delay = 60);
	void update() override;
	void draw()const override;
	void emitObject(Array<Object*>*) override;
};
