#include"Enemy.hpp"
class SwordZombie : public Enemy{
private:
    Vec2 playerpos;
    MapChip mapchip;
    double speed;
public:
    SwordZombie();
    void update(Vec2);
    void draw()const;
    void getmypos(Point);
    Vec2 sendmypos();
};
