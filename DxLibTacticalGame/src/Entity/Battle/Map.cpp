#include "Map.h"
#include "Entity/Unit/Unit.h"

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
				try {
					DxLib::DrawGraph(getRealX(x), getRealY(y), mass_[y][x].getImageId(), FALSE);
				}
				catch (std::out_of_range& oor) {
					std::cerr << "Out of Range: " << oor.what() << std::endl;
				}
				
			}
		}
	}

	/**
	 * @fn
	 * マスにユニットへの参照をセット
	 * @param (x) x座標（マス）
	 * @param (y) y座標（マス）
	 */
	void Map::setUnit(int x, int y, shared_ptr<Entity::Unit> unit)
	{
		try {
			mass_[y][x].setUnit(unit);
		}
		catch (std::out_of_range& oor) {
			std::cerr << "Out of Range: " << oor.what() << std::endl;
		}
		
	}

	void Map::onClickPlayerUnit(int x, int y)
	{
		if (deselectUnit()) // 選択解除
		{
			// ユニット選択
			shared_ptr<Entity::Unit> unit = mass_[getMassX(y)][getMassX(x)].getUnit();
			if (unit && unit->select(true))
			{
				selectedUnit_ = unit;
			}
		}
	}

	void Map::onClickMass(int x, int y)
	{
		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{
			// 移動
			selectedUnitSp->move(Entity::Map::getMassX(x), Entity::Map::getMassY(y));
		}
	}

	/**
	 * @fn
	 * ユニットの選択状態を解除
	 * @return 解除できた場合と選択済みのユニットがない場合はtrueを返す
	*/
	bool Map::deselectUnit()
	{
		if (selectedUnit_.expired())
		{
			return true;
		}
		shared_ptr<Entity::Unit> prevSelectedUnit = selectedUnit_.lock();
		if (prevSelectedUnit)
		{
			if (prevSelectedUnit->select(false))
			{
				selectedUnit_.reset();
				return true;
			}
			return false;
		}

		// 選択済みのユニットがない場合もtureを返す
		return true;
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