#pragma once
# include "Character.hpp"
# include "Scenes/Enemies/Enemy.hpp"
# include "Scenes/Objects/Object.hpp"

struct Blink
{
	// 点滅の速度
	int time;
};

class Player : public Character{
private:
	// テクスチャ読み込み
	const Texture CharacterTexture{ U"Sprites/player.png" };

	PixelShader ps;
	const PixelShader rgbShader = GLSL{ U"Shaders/rgb_shift.frag", {{U"PSConstants2D", 0}} };
	const PixelShader defaultShader = GLSL{ U"Shaders/default2d_texture.frag", {{U"PSConstants2D", 0}} };
	const PixelShader blinkShader = GLSL{ U"Shaders/blink.frag", {{U"PSConstants2D", 0}, {U"Blink", 1}} };

	ConstantBuffer<Blink> cb;

	int direction;
    int hp;
	int invinceT;


	int playerDirection;
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
	void spikeCollision();
	int32 spike(int32 chipIndex);

};
