﻿#include"SwordZombie.hpp"

SwordZombie::SwordZombie(Point mapPos){
    speed=0.25;
    velocity={0,0};
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
}
void SwordZombie::update(){
    if(pos.x<playerPos.x){
        velocity.x=speed;
    }
    if(pos.x>playerPos.x){
        velocity.x=-speed;
    }
    if(pos.y<playerPos.y){
        velocity.y=speed;
    }
    if(pos.y>playerPos.y){
        velocity.y=-speed;
    }

	moveRestriction();
	moveNextPosition();
}
void SwordZombie::draw() const {
    mapchip.get(5).draw(pos.x-collisionSize.x/2,pos.y-collisionSize.y/2);
}