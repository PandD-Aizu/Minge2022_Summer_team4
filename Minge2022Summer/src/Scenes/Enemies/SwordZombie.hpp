# pragma once
#include"Enemy.hpp"
class SwordZombie : public Enemy{
private:
    MapChip mapchip;
    double speed;
    double direction;
    int move;
    double timer;
public:
	int attackRange;
    SwordZombie(Point);
    void update()override;
    void draw()const override;
    bool isAttacking() const;
    void emitObject(Array <Object*>*) override {}
};
