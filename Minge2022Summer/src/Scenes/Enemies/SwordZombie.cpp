#include"SwordZombie.hpp"
SwordZombie::SwordZombie(){
    playerpos={1000,1000};
    speed=0.25;
    characterVelocity={0,0};
}
void SwordZombie::update(){
    if(characterPosition.x<playerpos.x){
        characterVelocity.x=speed;
    }
    if(characterPosition.x>playerpos.x){
        characterVelocity.x=-speed;
    }
    if(characterPosition.y<playerpos.y){
        characterVelocity.y=speed;
    }
    if(characterPosition.y>playerpos.y){
        characterVelocity.y=-speed;
    }
}
void SwordZombie::draw()const{
    mapchip.get(5).draw(characterPosition.x-characterCollisionSize.x/2,characterPosition.y-characterCollisionSize.y/2);
}
void SwordZombie::getmypos(Point pos){
    characterPosition={pos.x+characterCollisionSize.x/2,pos.y+characterCollisionSize.y/2};
}
void SwordZombie::getplayerpos(Vec2 pos){
    playerpos=pos;
}
