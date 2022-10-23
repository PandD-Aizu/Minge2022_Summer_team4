#include"Bomber.hpp"

Bomber::Bomber(Point mapPos)
	:Enemy(U"Sprites/bomber.png")
{
    speed=0.15;
    velocity={0,0};
	pos = { mapPos.x + collisionSize.x / 2, mapPos.y + collisionSize.y / 2 };
}
void Bomber::update(){
	plantCnt--;
	

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

	ensureDirection();
	moveRestriction();
	moveNextPosition();
}
void Bomber::draw() const {
	Enemy::draw();
    // mapchip.get(6).draw(pos.x-collisionSize.x/2,pos.y-collisionSize.y/2);
}

void Bomber::emitObject(Array <Object *> *objects) {
	if (plantCnt <= 0) {
		*(objects) << new Bomb(pos); // 敵用の爆弾の設置
		plantCnt = 500;
	}
}
