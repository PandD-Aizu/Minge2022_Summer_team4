#pragma once
#include <Siv3D.hpp>

struct GameData {
	bool isMapSelected = false;
};

using App = SceneManager<String, GameData>;
