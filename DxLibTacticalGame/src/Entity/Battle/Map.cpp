#include "Map.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Map::Map()
	{
		// テスト処理
		w_ = 20;
		h_ = 10;

		shape_.set(0, 0, w_ * CHIP_SIZE, h_ * CHIP_SIZE);

		mass_.reserve(h_); // メモリ確保

		// マップデータ生成
		for (int y = 0; y < h_; y++)
		{
			mass_.push_back(vector<Mass>());
			mass_[y].reserve(w_); // メモリ確保
			
			for (int x = 0; x < w_; x++)
			{
				int kind = Mass::Kind::PLAIN;

				if (x == 0 || x == w_ - 1)
				{
					kind = Mass::Kind::MOUNTAIN;
				}
				else if (y == 5)
				{
					kind = Mass::Kind::RIVER;
				}
				else if (x == 5)
				{
					kind = Mass::Kind::FOREST;
				}

				mass_[y].push_back(Mass(kind));
			}
		}
	}

	/**
	 * @fn
	 * 描画
	 */
	void Map::render() const
	{
		for (int y = 0; y < h_; y++)
		{
			for (int x = 0; x < w_; x++)
			{
				DxLib::DrawGraph(x * CHIP_SIZE, y * CHIP_SIZE, mass_[y][x].getImageId(), FALSE);
			}
		}
	}

}