#include"SwordZombie.hpp"

SwordZombie::SwordZombie(Point mapPos)
	:Enemy{ U"Sprites/swordzombie.png"}
{
    speed=0.25;
    velocity={0,0};
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
}
void SwordZombie::update(){
	velocity = Vec2{ 0, 0 };
    if((pos.x + speed/2)<playerPos.x){
        velocity.x=speed;
    }
    if((pos.x - speed/2) >playerPos.x){
        velocity.x=-speed;
    }
    if((pos.y + speed/2) <playerPos.y){
        velocity.y=speed;
    }
    if((pos.y - speed/2)>playerPos.y){
        velocity.y=-speed;
    }

	ensureDirection();
	moveRestriction();
	moveNextPosition();
}
void SwordZombie::draw() const {
	Enemy::draw();
    // mapchip.get(5).draw(pos.x-collisionSize.x/2,pos.y-collisionSize.y/2);
}
