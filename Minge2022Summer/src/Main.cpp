//# include <Siv3D.hpp>
# include "common.hpp"
# include "Scenes/Stage1.hpp"
# include "Scenes/Stage2.hpp"
# include "Scenes/Stage3.hpp"
# include "Scenes/Title.hpp"
# include "Scenes/GameClear.hpp"
# include "Scenes/GameOver.hpp"

SIV3D_SET(EngineOption::Renderer::OpenGL);

Grid<int> mapLayer0;
Grid<int> mapLayer1;

void Main()
{

	// アセットの登録
	AudioAsset::Register(U"slash0", U"Sounds/SE/slash 01.wav");
	AudioAsset::Register(U"slash1", U"Sounds/SE/slash 02.wav");
	AudioAsset::Register(U"slash2", U"Sounds/SE/slash 03.wav");
	AudioAsset::Register(U"slash3", U"Sounds/SE/slash 04.wav");
	AudioAsset::Register(U"SZ_attack", U"Sounds/SE/SwordZombie_attack.mp3");
	AudioAsset::Register(U"SZ_hit", U"Sounds/SE/SwordZombie_hit.mp3");
	AudioAsset::Register(U"homingShot", U"Sounds/SE/HomingGun.mp3");
	AudioAsset::Register(U"explode", U"Sounds/SE/explode.wav");
	AudioAsset::Register(U"warp", U"Sounds/SE/warp.mp3");
	// AudioAsset::Register(U"", U"Sounds/SE");
	AudioAsset::Register(U"arrowShot", U"Sounds/SE/arrowShot.wav");
	AudioAsset::Register(U"arrowHit", U"Sounds/SE/arrowHit.mp3");

	AudioAsset::Register(U"mainBGM", U"Sounds/BGM/Unchangeable.mp3");
	AudioAsset(U"mainBGM").setLoop(true);

	// ウィンドウを 640x480 にリサイズする
	//Scene::SetResizeMode(ResizeMode::Keep);
	Window::Resize(640, 480);
	Scene::SetTextureFilter(TextureFilter::Nearest);

	//FontAsset::Register(U"TitleFont", 60, Typeface::Heavy);

	// シーンマネージャーを作成
	App manager;

	// タイトルシーン（名前は "Title"）を登録
	manager.add<Title>(U"Title");

	// ゲームシーン（名前は "Stage"）を登録
	manager.add<Stage1>(U"Stage1");
	manager.add<Stage2>(U"Stage2");
	manager.add<Stage3>(U"Stage3");

	// ゲームクリアーシーン（名前は "GameClear"）を登録
	manager.add<GameClear>(U"GameClear");

	manager.add<GameOver>(U"GameOver");

	int FPS = 60; // 1秒間に1画面を書き換える回数
	Stopwatch sw;   //FPS60
	sw.start(); //FPS60
	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
		while (sw.msF() < 1000.0 / FPS);    //1/60秒経過するまでループ
		sw.restart();   //FPS60  ストップウォッチをリスタート
	}
}
