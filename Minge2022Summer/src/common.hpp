#pragma once
#include <Siv3D.hpp>

Grid<int32> LoadCSV(FilePathView path);

struct GameData {
	bool isMapSelected = false;
};

using App = SceneManager<String, GameData>;

Grid<int32> LoadCSV(FilePathView path);

Grid<int32> mapLayer0,mapLayer1;
Size MapSize;


class MapChip
{
public:
    MapChip();
    // マップチップのサイズ（ピクセル）
    static constexpr int32 MapChipSize = 16;
    
    // 指定したインデックスのマップチップを返す
    TextureRegion get(int32) const;

private:

    Texture m_base{U"blocks.png"};
};

