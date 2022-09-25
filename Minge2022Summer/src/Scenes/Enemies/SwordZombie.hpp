#include"../../Character.hpp"
class SwordZombie : public Character{
private:
    Vec2 playerpos;
    MapChip mapchip;
    double speed;
    bool touchplayer;
public:
    SwordZombie();
    void update();
    void draw()const;
    void getmypos(Point);
    void getplayerpos(Vec2);
};
