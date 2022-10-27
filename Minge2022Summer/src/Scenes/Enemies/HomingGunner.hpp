# pragma once
#include"Enemy.hpp"
class HomingGunner : public Enemy {
private:
	double speed;
	int shotCnt = 100;
	int maxShotCnt;
public:
	HomingGunner(Point, int);
	void update() override;
	void draw()const override;
	void emitObject(Array<Object*>*) override;
};
