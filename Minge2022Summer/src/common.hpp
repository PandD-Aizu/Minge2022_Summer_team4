#pragma once
#include <Siv3D.hpp>

Grid<int32> LoadCSV(FilePathView);

struct GameData {
	bool isMapSelected = false;
};

extern Grid<int> mapLayer0;
extern Grid<int> mapLayer1;

using App = SceneManager<String, GameData>;

class MapChip
{
public:

	// マップチップのサイズ（ピクセル）
	static constexpr int32 MapChipSize = 16;

	MapChip();

	// 指定したインデックスのマップチップを返す
	TextureRegion get(int32 chipIndex) const;

	// とげだった場合にインターバルとかの処理を行う
	int32 toge(int32 chipIndex) const;

private:

	Texture m_base;
};
