#include "common.hpp"
# define STAIRS 10

MapChip::MapChip() {}

	// 指定したインデックスのマップチップを返す
TextureRegion MapChip::get(int32 chipIndex) const
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

