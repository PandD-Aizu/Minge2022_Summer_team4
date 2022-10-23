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
    SwordZombie(Point);
    void update()override;
    void draw()const override;
    bool isAttacking();
    void emitObject(Array <Object*>*) override {}
};
