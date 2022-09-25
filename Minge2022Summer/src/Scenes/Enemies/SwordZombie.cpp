#include"SwordZombie.hpp"
SwordZombie::SwordZombie(){
    playerpos={1000,1000};
    speed=0.25;
    velocity={0,0};
    touchplayer=false;
}
void SwordZombie::update(){
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
    if(pos.distanceFrom(playerpos)<8){
        touchplayer=true;
    }
}
void SwordZombie::draw()const{
    mapchip.get(5).draw(pos.x-collisionSize.x/2,pos.y-collisionSize.y/2);
}
void SwordZombie::getmypos(Point mypos){
    pos={mypos.x+collisionSize.x/2,mypos.y+collisionSize.y/2};
}
void SwordZombie::getplayerpos(Vec2 pos){
    playerpos=pos;
}
