#pragma once
# include "../common.hpp"
# include "../Character.hpp"
# include "myCamera.hpp"
# include "Objects/Object.hpp"
# include "../Player.hpp"
# include "Enemies/Bomber.hpp"
# include "Enemies/SwordZombie.hpp"
# include "Enemies/HomingGunner.hpp"
# include "Enemies/BounceGunner.hpp"
# include "Enemies/ArcherWall.hpp"


class Game: public App::Scene {
private:
	MyCamera camera;
	// マップチップ用テクスチャ
	MapChip mapchip;

	RenderTexture renderTexture;

    int32 countswordzombies;
	// マップのセルの数（横 20, 縦 15 マス）
    Array<Enemy*> enemies;
	Array<Object*> objects;

	Player player;
    
	// ゲームクリア領域
	Circle gameClearBody{ Vec2{ 16 * 16 + 8, 16 * 4 + 8}, 8 };


public:
	Game(const InitData& init);

	void update() override;
	void draw() const override;
};
