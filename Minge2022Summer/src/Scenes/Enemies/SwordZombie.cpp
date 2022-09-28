#include"SwordZombie.hpp"
SwordZombie::SwordZombie(){
    playerpos={1000,1000};
    speed=0.25;
    velocity={0,0};
}
void SwordZombie::update(Vec2 playerpos){
    if(pos.x<playerpos.x){
        velocity.x=speed;
    }
    if(pos.x>playerpos.x){
        velocity.x=-speed;
    }
    if(pos.y<playerpos.y){
        velocity.y=speed;
    }
    if(pos.y>playerpos.y){
        velocity.y=-speed;
    }
}
void SwordZombie::draw()const{
    mapchip.get(5).draw(pos.x-collisionSize.x/2,pos.y-collisionSize.y/2);
}
void SwordZombie::getmypos(Point mypos){
    pos={mypos.x+collisionSize.x/2,mypos.y+collisionSize.y/2};
}
Vec2 SwordZombie::sendmypos(){
    return pos;
}
