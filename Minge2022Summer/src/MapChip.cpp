# include "common.hpp"

#define STAIRS 10

MapChip::MapChip()
		: m_base{ U"blocks.png" } {}

// 指定したインデックスのマップチップを返す
TextureRegion MapChip::get(int32 chipIndex) const
{
	// インデックスが30番台なら3番のマップチップ、インデックスが40番台なら4番のマップチップ
	if (chipIndex / STAIRS == 3) {
		chipIndex = 3;
	}
	else if (chipIndex / STAIRS == 4) {
		chipIndex = 4;
	}

	// インデックスが10番台なら6番のマップチップ（現在5番はソードゾンビに利用されている。場合によってはそこをとげにする）
	if (chipIndex / 10 == 1) {
		chipIndex = spike(chipIndex);
	}

	const int32 x = ((chipIndex % 8) * MapChipSize);
	const int32 y = ((chipIndex / 8) * MapChipSize);
	return m_base(x, y, MapChipSize);
}

int32 MapChip::spike(int32 chipIndex) const
{
	if (chipIndex / 10 == 1) {
		if (chipIndex % 10 != 0) {
			chipIndex = static_cast<int32>(Scene::Time() * 1 / (chipIndex % 10)) % 2 + 3;
		}
		else {
			chipIndex = 4;
		}
	}

	return chipIndex;
}

bool isTileExist(Vec2 pos) {
	Point targetCell(static_cast<int32>(pos.x / MapChip::MapChipSize), static_cast<int32>(pos.y / MapChip::MapChipSize));
	if (mapLayer1[targetCell.y][targetCell.x] == 2) {
		ClearPrint();
		Print << U"targetCell" << targetCell;
		return true;
	}
	else return false;
}

