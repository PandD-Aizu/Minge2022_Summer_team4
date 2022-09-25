﻿#include"../../Character.hpp"
class SwordZombie : public Character{
private:
    Vec2 playerpos;
    MapChip mapchip;
    double speed;
public:
    SwordZombie();
    void update();
    void draw()const;
    void getmypos(Point);
    void getplayerpos(Vec2);
    Vec2 sendmypos();
};
