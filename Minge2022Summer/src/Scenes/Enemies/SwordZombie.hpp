# pragma once
#include"Enemy.hpp"
class SwordZombie : public Enemy{
private:
    MapChip mapchip;
    double speed;
public:
    SwordZombie(Point);
    void update() override;
    void draw()const override;
};
