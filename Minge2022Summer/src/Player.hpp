#pragma once
# include "Character.hpp"

class Player : public Character{
private:
	// テクスチャ読み込み
	const Texture CharacterTexture{ U"playerSprite.png" };

public:
	void drawWalk() const;		//プレイヤーの歩行の描画

};
