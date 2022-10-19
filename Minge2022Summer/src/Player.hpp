#pragma once
# include "Character.hpp"
# include "Scenes/Enemies/Enemy.hpp"
# include "Scenes/Objects/Object.hpp"

#define MAXENEMIESNUM 100

class Player : public Character{
private:
	// テクスチャ読み込み
	const Texture CharacterTexture{ U"playerSprite.png" };

	int direction;
    
    int hp;
    Vec2 enemiespos[MAXENEMIESNUM];
public:
    Player();
    void update();
	void draw() const;		//プレイヤーの歩行の描画
    bool died();
	void decideDirection();
	void detectEnemyCollision(Enemy*);
	void changeDirection();		//プレイヤーの向きをマウスの方向に変更する
	void detectObjCollision(Object*);

};
