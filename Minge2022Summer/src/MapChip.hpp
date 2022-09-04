#pragma once

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
		const int32 x = ((chipIndex % 8) * MapChipSize);
		const int32 y = ((chipIndex / 8) * MapChipSize);
		return m_base(x, y, MapChipSize);
	}

private:

	Texture m_base;
};
