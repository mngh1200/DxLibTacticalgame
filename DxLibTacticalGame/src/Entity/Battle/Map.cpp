#include "Map.h"
#include "Entity/Unit/Unit.h"

namespace Entity {
	/**
	 * @fn
	 * コンストラクタ
	 */
	Map::Map()
	{
		weak_ptr<Unit> tmpUnit;

		if (tmpUnit.expired()) {
			int a = 10;
		}

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
				try {
					DxLib::DrawGraph(getRealX(x), getRealY(y), mass_[y][x].getImageId(), FALSE);
				}
				catch (std::out_of_range& oor) {
					std::cerr << "Out of Range: " << oor.what() << std::endl;
				}
				
			}
		}
	}

	bool Map::isRange(int x, int y) const
	{
		return 0 <= x && x <= w_ && 0 <= y && y <= h_;
	}

	/**
	 * @fn
	 * マスの座標から正味の座標を取得(x座標)
	 */
	int Map::getRealX(int massX)
	{
		return massX * CHIP_SIZE;
	}

	/**
	 * @fn
	 * マスの座標から正味の座標を取得(y座標)
	 */
	int Map::getRealY(int massY)
	{
		return massY * CHIP_SIZE;
	}

	/**
	 * @fn
	 * 正味の座標からマスの座標を取得(x座標)
	 */
	int Map::getMassX(int realX)
	{
		return realX / CHIP_SIZE;
	}

	/**
	 * @fn
	 * 正味の座標からマスの座標を取得(y座標)
	 */
	int Map::getMassY(int realY)
	{
		return realY / CHIP_SIZE;
	}

}