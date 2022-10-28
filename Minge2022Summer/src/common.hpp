#pragma once
#include <Siv3D.hpp>

Grid<int32> LoadCSV(FilePathView);

// 指定したpositionに壁が存在するかどうか返す
bool isTileExist(Vec2);

struct GameData {
	bool isMapSelected = false;
    int currentStage = 1;
};

extern Grid<int> mapLayer0;
extern Grid<int> mapLayer1;
extern Size MapSize;

using App = SceneManager<String, GameData>;

class MapChip
{
public:

	// マップチップのサイズ（ピクセル）
	static constexpr int32 MapChipSize = 32;

	MapChip();

	// 指定したインデックスのマップチップを返す
	TextureRegion get(int32 chipIndex) const;

	// とげだった場合にインターバルとかの処理を行う
	int32 spike(int32 chipIndex) const;

private:

	Texture m_base;
};
