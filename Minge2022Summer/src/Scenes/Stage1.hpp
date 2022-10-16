#pragma once
# include "../common.hpp"
# include "../Character.hpp"
# include "myCamera.hpp"
# include "Objects/Stair.hpp"
# include "../Player.hpp"
# include "Enemies/SwordZombie.hpp"

#define MAXSWORDZOMBIESNUM 10



class Stage1 : public App::Scene {
private:
	MyCamera camera;
	// マップチップ用テクスチャ
	MapChip mapchip;
	RenderTexture renderTexture;

	Array<Stair*> stairs;
    Array<Enemy*> enemies;

	Player player;
    
	// ゲームクリア領域
	Circle gameClearBody{ Vec2{ 16 * 16 + 8, 16 * 4 + 8}, 8 };


public:
	Stage1(const InitData& init);

	void update() override;
	void draw() const override;
};
