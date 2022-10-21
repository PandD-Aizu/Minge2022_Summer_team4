#include"Bomber.hpp"

Bomber::Bomber(Point mapPos){
    speed=0.25;
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

	moveRestriction();
	moveNextPosition();
}
void Bomber::draw() const {
    mapchip.get(5).draw(pos.x-collisionSize.x/2,pos.y-collisionSize.y/2);
}

void Bomber::emitObject(Array <Object *> objects) {
	if (plantCnt <= 0) {
		objects << new Bomb(pos); // 敵用の爆弾の設置
		plantCnt = 100;
	}
}
