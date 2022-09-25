# include "Player.hpp"
Player::Player(){
    hp=1;
    for(int32 i=0;i<MAXENEMIESNUM;i++){
        enemiespos[i]={1000,1000};
    }
}
void Player::update(){
    for(int32 i=0;i<MAXENEMIESNUM;i++){
        if(enemiespos[i].distanceFrom(pos)<16){
            hp--;
        }
    }
}
void Player::getenemiespos(Vec2 pos[MAXENEMIESNUM]){
    for(int32 i=0;i<MAXENEMIESNUM;i++){
        enemiespos[i]=pos[i];
    }
}
void Player::drawWalk() const {
	// 歩行のアニメーションのインデックス(x, y)
	Vec2 animationIndex{ 0, 0 };

	// 横方向のインデックス（歩行モーション）
	if (direction == 4) animationIndex.x = 0;
	else animationIndex.x = static_cast<int32>(Scene::Time() * 10) % 6;

	// 縦方向のインデックス（プレイヤーの向き）
	// プレイヤーが移動中でない場合は、最後に向いていた方向を使用する
	if (direction == 4) animationIndex.y = lastDirection;
	else animationIndex.y = direction;

	// 描画
	CharacterTexture((textureSize.x * animationIndex.x), (textureSize.y * animationIndex.y), textureSize.x, textureSize.y)
		.draw(
			  pos.x - textureCenter.x,
					pos.y - textureCenter.y
		);
}
bool Player::died(){
    if(hp<=0){
        return true;
    }else{
        return false;
    }
}
