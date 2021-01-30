#include "EnemyAI.h"

namespace Battle {
	/**
	 * @fn
	 * 初期処理
	 * @param (map) mapポインタ
	 */
	void EnemyAI::init(shared_ptr<Map> map)
	{
		setBaseScore(map);
	}

	/**
	 * @fn
	 * 操作手順を返す
	 * @param (map) mapポインタ
	 */
	bool EnemyAI::createOrders(shared_ptr<Map> map)
	{
		shared_ptr<Unit> unit = getNextUnit(map);

		if (!unit)
		{
			return false; // 未操作のユニットがない場合は終了
		}

		// 移動可能範囲取得
		vector<pair<int, int>> movPosList;
		map->getMovableMasses(unit, unit->getMassX(), unit->getMassY(), unit->getInfo().mov, movPosList);

		int maxPoint = 0; // 移動対象マスの得点 
		pair<int, int> targetPos = make_pair(unit->getMassX(), unit->getMassY()); // 移動対象マス
		shared_ptr<Unit> targetUnit; // 攻撃対象ユニット

		for (auto itr = movPosList.begin(); itr != movPosList.end(); ++itr)
		{
			// 対象マスの得点計算
			int point = 0;

			// 戦闘予測からのポイント算出
			shared_ptr<Unit> tmpTargetUnit;
			point += getMassPoint(map, unit, itr->first, itr->second, tmpTargetUnit);

			// 得点が高ければ移動対象マスに指定
			if (point > maxPoint)
			{
				targetPos = *itr;
				maxPoint = point;
				targetUnit = tmpTargetUnit;
			}
		}

		// 移動命令
		orders_.push_back(Order{Order::Kind::MOVE, unit, targetPos.first, targetPos.second});

		if (targetUnit)
		{
			// 攻撃命令
			orders_.push_back(Order{ Order::Kind::ATACK, unit, targetUnit->getMassX(), targetUnit->getMassY()});
		}
		else
		{
			// 待機命令
			orders_.push_back(Order{ Order::Kind::WAIT});
		}

		return true;
	}

	/**
	 * @fn
	 * 対象マスの得点を返す
	 * @param (map) mapポインタ
	 * @param (unit) 対象ユニット
	 * @param (x) 攻撃起点のマスX
	 * @param (y) 攻撃起点のマスY
	 * @param (targetUnit) 最大得点の攻撃対象ユニットを返すための参照
	 * @return 得点
	 */
	int EnemyAI::getMassPoint(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y, shared_ptr<Unit>& targetUnit)
	{
		int point = 0;

		// 攻撃可能マス取得
		vector<pair<int, int>> atkPosList;
		map->getAtackAbleMasses(unit, x, y, atkPosList, true);

		Fight fight = Fight();
		fight.init(map);

		if (atkPosList.size() > 0)
		{
			for (auto itr = atkPosList.begin(); itr != atkPosList.end(); ++itr)
			{
				// 対象マス攻撃時の得点計算
				int tmpPoint = 0;

				int targetMassX = itr->first;
				int targetMassY = itr->second;

				shared_ptr<Unit> tmpTargetUnit = map->getUnit(targetMassX, targetMassY);

				fight.setPrepare(unit, x, y, tmpTargetUnit);

				FightData actFight = fight.getFightData(true);
				FightData defFight = fight.getFightData(false);

				tmpPoint = actFight.damage * actFight.hitRate / 100;
				tmpPoint -= defFight.damage * defFight.hitRate / 100;

				if (tmpPoint > point) // 得点が高い場合
				{
					targetUnit = tmpTargetUnit;
					point = tmpPoint;
				}
			}
		}

		// 基本スコアを加算
		if (massBaseScoreMap.count(make_pair(x, y)) != 0)
		{
			point += massBaseScoreMap.at(make_pair(x, y));
		}

		return point;
	}


	/**
	 * @fn
	 * 最新の操作内容を返す（最新の操作内容をリストから削除）
	 * @return 操作内容（存在しない場合は）
	 */
	Order EnemyAI::getNextOrder()
	{
		if (orders_.size() > 0)
		{
			Order order = orders_.at(0);
			orders_.erase(orders_.begin());
			return order;
		}
		return Order();
	}

	/**
	 * @fn
	 * 各マス座標の基本スコアをセット
	 * @param (map) mapポインタの参照
	 */
	void EnemyAI::setBaseScore(shared_ptr<Map> map)
	{
		int y = 0;
		int x = 0;

		for (auto yItr = map->massBegin(); yItr != map->massEnd(); ++yItr)
		{
			auto lineItr = *yItr;
			for (auto xItr = lineItr.begin(); xItr != lineItr.end(); ++xItr)
			{
				int kind = (*xItr)->getKind();

				if (kind == Mass::Kind::FORT_ENEMY) // AI側砦
				{
					massBaseScoreMap.emplace(make_pair(x, y), 100);
				}
				else if (kind == Mass::Kind::FORT_PLAYER) // プレイヤー砦
				{
					massBaseScoreMap.emplace(make_pair(x, y), 200);
				}
				++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * 次に操作するユニットを取得
	 * @param (map) mapポインタの参照
	 * @return 捜査対象ユニット
	 */
	shared_ptr<Unit> EnemyAI::getNextUnit(shared_ptr<Map> map)
	{
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			shared_ptr<Unit> unit = itr->second;
			if (unit->isEnemy() && !unit->isActed())
			{
				return unit;
			}
		}
		return shared_ptr<Unit>();
	}

}