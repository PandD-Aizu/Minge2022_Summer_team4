#include"SwordZombie.hpp"
#include"Math.h"

SwordZombie::SwordZombie(Point mapPos){
    speed=Random(0.2,0.4);
    velocity={0,0};
	pos = {mapPos.x+collisionSize.x/2,mapPos.y+collisionSize.y/2};
    direction=0;
    move=1;
    timer=0;
	attackRange = 15;
}
void SwordZombie::update(){
    direction=atan2(pos.y-playerPos.y,pos.x-playerPos.x)-90_deg;
    velocity.x=move*speed*sin(direction);
    velocity.y=move*speed*-cos(direction);
    if(pos.distanceFrom(playerPos)<17){
        move=0;
        timer+=Scene::DeltaTime();
        if(timer>=1){
            timer=0;
        }
    }else{
        move=1;
        timer=0;
    }
	moveRestriction();
	moveNextPosition();
}
void SwordZombie::draw()const{
    mapchip.get(5).draw(pos.x-collisionSize.x/2,pos.y-collisionSize.y/2);
    if(timer>=0.8){
        Circle{pos.x,pos.y,8}.drawArc(direction-45_deg,90_deg,0, attackRange,ColorF{1,0,0,0.5});
    }
}
bool SwordZombie::isAttacking(){
    if(timer>=0.8){
        return true;
    }else{
        return false;
    }
}
