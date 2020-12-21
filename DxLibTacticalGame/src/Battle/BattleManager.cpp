#include "BattleManager.h"

namespace Battle {
	/**
	 * @fn
	 * コンストラクタ
	 */
	BattleManager::BattleManager(shared_ptr<Entity::Map> map)
	{
		map_ = map;
	}

	/**
	 * @fn
	 * マスにユニットへの参照をセット
	 * @param (unit) ユニットの参照
	 * @return 追加成功 : true
	 */
	bool BattleManager::setUnit(shared_ptr<Entity::Unit> unit)
	{
		int x = unit->getX();
		int y = unit->getY();

		if (map_->isRange(x, y))
		{
			auto ret = mapUnits_.emplace(make_pair(x, y), unit); // 新規追加のみ
			return ret.second;
		}
		return false;
	}

	void BattleManager::onClickPlayerUnit(int x, int y)
	{
		if (deselectUnit()) // 選択解除
		{
			// ユニット選択
			shared_ptr<Entity::Unit> unit = getUnitWp(Map::getMassX(x), Map::getMassY(y)).lock();
			if (unit && unit->select(true))
			{
				selectedUnit_ = unit;
			}
		}
	}

	void BattleManager::onClickMass(int x, int y)
	{
		shared_ptr<Entity::Unit> selectedUnitSp = selectedUnit_.lock();
		if (selectedUnitSp)
		{
			// 移動
			selectedUnitSp->move(Map::getMassX(x), Map::getMassY(y));
		}
	}

	weak_ptr<Unit> BattleManager::getUnitWp(int massX, int massY)
	{
		try
		{
			return mapUnits_.at(make_pair(massX, massY));
		}
		catch (out_of_range&) {}

		return weak_ptr<Unit>();
	}

	/**
	 * @fn
	 * ユニットの選択状態を解除
	 * @return 解除できた場合と選択済みのユニットがない場合はtrueを返す
	*/
	bool BattleManager::deselectUnit()
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
}