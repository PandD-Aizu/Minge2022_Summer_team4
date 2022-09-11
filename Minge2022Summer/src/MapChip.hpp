#pragma once
# define STAIRS 10

class MapChip
{
public:

	// マップチップのサイズ（ピクセル）
	static constexpr int32 MapChipSize = 16;

	MapChip()
		: m_base{ U"blocks.png" } {}

	// 指定したインデックスのマップチップを返す
	TextureRegion get(int32 chipIndex) const
	{
        // インデックスが30番台なら3番のマップチップ、インデックスが40番台なら4番のマップチップ
        if(chipIndex / STAIRS == 3){
            chipIndex = 3;
        }else if(chipIndex / STAIRS == 4){
            chipIndex = 4;
        }
        
		const int32 x = ((chipIndex % 8) * MapChipSize);
		const int32 y = ((chipIndex / 8) * MapChipSize);
		return m_base(x, y, MapChipSize);
	}

private:

	Texture m_base;
};
