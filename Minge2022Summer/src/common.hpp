#pragma once
#include <Siv3D.hpp>

Grid<int32> LoadCSV(FilePathView path);

struct GameData {
	bool isMapSelected = false;
};

using App = SceneManager<String, GameData>;



