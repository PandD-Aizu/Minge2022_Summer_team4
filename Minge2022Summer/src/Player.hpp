#pragma once
# include "Character.hpp"
# include "Scenes/Enemies/Enemy.hpp"
# include "Scenes/Objects/Object.hpp"

class Player : public Character{
private:
	// テクスチャ読み込み
	const Texture CharacterTexture{ U"playerSprite.png" };

	PixelShader ps;
	const PixelShader rgbShader = HLSL{ U"Shaders/rgb_shift.hlsl", U"PS" };
	const PixelShader defaultShader = HLSL{ U"Shaders/default2d.hlsl", U"PS_Texture" };

	int direction;
    int hp;
	int invinceT;

public:
    Player();
    void update();
	void draw() const;		//プレイヤーの歩行の描画
	void damaged();
    bool died();
	void decideDirection();
	void detectEnemyCollision(Enemy*);
	void changeDirection();		//プレイヤーの向きをマウスの方向に変更する
	void detectObjCollision(Object*);

};
