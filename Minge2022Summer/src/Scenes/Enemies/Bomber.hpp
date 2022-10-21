# pragma once
#include"Enemy.hpp"
class Bomber : public Enemy{
private:
    MapChip mapchip;
    double speed;
	double plantCnt;
public:
    Bomber(Point);
    void update() override;
    void draw()const override;
	void emitObject(Array<Object *>) override;
};
