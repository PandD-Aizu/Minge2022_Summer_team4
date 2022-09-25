#pragma once
# include "Character.hpp"
#define MAXENEMIESNUM 100

class Player : public Character{
private:
	// テクスチャ読み込み
	const Texture CharacterTexture{ U"playerSprite.png" };
    
    int hp;
    Vec2 enemiespos[MAXENEMIESNUM];
public:
    Player();
    void update();
    void getenemiespos(Vec2[]);
	void drawWalk() const;		//プレイヤーの歩行の描画
    bool died();

};
