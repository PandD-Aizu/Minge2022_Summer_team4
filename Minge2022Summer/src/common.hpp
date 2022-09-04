#pragma once
#include <Siv3D.hpp>
//#define DEBUG
class Tile;

struct GameData {
	bool isMapSelected = false;
	FilePath selectedMap;

	bool isMapSet = false;
	Array<Array<Tile>> map;
	Point startPoint;
	Point goalPoint;
	SecondsF playerTime = 60.0s;		// プレイヤーシーンの制限時間
	SecondsF enemyTime = 25.0s;			// エネミーシーンの制限時間
};

using App = SceneManager<String, GameData>;
