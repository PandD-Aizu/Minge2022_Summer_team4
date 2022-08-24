#pragma once

// CSV ファイルから int32 の二次元配列を読み込む
Grid<int32> LoadCSV(FilePathView path)
{
	// CSC ファイルを読み込む
	CSV csv{ path };

	// 読み込みに失敗したら
	if (not csv)
	{
		// エラー
		throw Error{ U"Failed to load `{}`"_fmt(path) };
	}

	// 行数
	const size_t yCount = csv.rows();

	// 1 行目の列数（以降も同じ列数と仮定する）
	const size_t xCount = csv.columns(0);

	// 二次元配列
	Grid<int32> map(xCount, yCount);

	for (size_t y = 0; y < yCount; ++y)
	{
		for (size_t x = 0; x < xCount; ++x)
		{
			// 指定した行・列の値を読み込む
			map[y][x] = csv.get<int32>(y, x);
		}
	}

	return map;
}
