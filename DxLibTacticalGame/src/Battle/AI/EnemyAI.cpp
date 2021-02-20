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
	 * AIの志向特性をセットする
	 * @param (oriented) 志向特性
	 */
	void EnemyAI::setOriented(Oriented oriented)
	{
		oriented_ = oriented;
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

			// 現在マスに留まる志向スコア
			if (oriented_.stay != 0 && itr->first == unit->getMassX() && itr->second == unit->getMassY())
			{
				point += oriented_.stay;
			}

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
	 * 自分以外の味方ユニットがいるか判定
	 * @param (unit) 基準のユニット
	 * @param (x) 対象のx座標
	 * @param (y) 対象のy座標
	 * @return 隣接時 trueを返す
	 */
	bool isFriendExist(shared_ptr<Map> map, shared_ptr<Unit> unit, int x, int y)
	{
		shared_ptr<Unit> targetUnit = map->getUnit(x, y);
		if (targetUnit && targetUnit != unit && targetUnit->isEnemy() == unit->isEnemy())
		{
			return true;
		}
		return false;
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

				tmpPoint = (actFight.damage * actFight.hitRate / 100) * oriented_.atack;
				tmpPoint -= (defFight.damage * defFight.hitRate / 100)* oriented_.survive;

				if (tmpPoint > point) // 得点が高い場合
				{
					targetUnit = tmpTargetUnit;
					point = tmpPoint;
				}
			}
		}

		// 味方ユニットとの隣接ボーナス
		int adjacentCount = 0; // 隣接数

		if (isFriendExist(map, unit, x, y + 1))
		{
			++adjacentCount;
		}

		if (isFriendExist(map, unit, x, y - 1))
		{
			++adjacentCount;
		}

		if (isFriendExist(map, unit, x + 1, y))
		{
			++adjacentCount;
		}

		if (isFriendExist(map, unit, x - 1, y))
		{
			++adjacentCount;
		}

		point += adjacentCount * SCORE_BY_FRIEND * oriented_.friendship;

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

				if (kind == Mass::Kind::FORT_ENEMY || kind == Mass::Kind::FORT_PLAYER) // 砦マス
				{
					std::map<pair<int, int>, int> tmpBaseScoreMap;
					massBaseScoreMap[make_pair(x, y)] = FORT_SCORE * oriented_.targetPoint;

					setBaseScoreByFort(map, x, y, SCORE_BY_FORT * oriented_.targetPoint, tmpBaseScoreMap);
					map->clearMassState();

					// 砦から波及する基本スコアをセット
					for (auto mapItr = tmpBaseScoreMap.begin(); mapItr != tmpBaseScoreMap.end(); ++mapItr)
					{
						auto key = mapItr->first;
						auto val = mapItr->second;
						if (massBaseScoreMap.count(key) == 0) // 未指定の場合はセット
						{
							massBaseScoreMap.emplace(key, val);
						}
						else // すでにある場合は加算
						{
							massBaseScoreMap[key] += val;
						}
					}
				}

				++x;
			}
			++y;
			x = 0;
		}
	}

	/**
	 * @fn
	 * 砦マスの周りに波及する基本スコアの上昇分を追加
	 * @param (map) mapポインタの参照
	 */
	void EnemyAI::setBaseScoreByFort(shared_ptr<Map> map, int x, int y, int move, std::map<pair<int, int>, int>& tmpBaseScoreMap)
	{
		shared_ptr<Mass> nowMass = map->getMass(x, y);

		// マップ外
		if (nowMass->getKind() == Mass::Kind::OUT_OF_MAP)
		{
			return;
		}

		if (nowMass->getCost() == INT_MAX)
		{
			move = 0;
		}
		else
		{
			move = move - nowMass->getCost() * FORT_MOVE_SCORE_RATE * oriented_.targetPoint;
		}
		
		if (move > nowMass->passingMov)
		{
			nowMass->passingMov = move;
			tmpBaseScoreMap[make_pair(x, y)] =  move;

			setBaseScoreByFort(map, x, y + 1, move, tmpBaseScoreMap);
			setBaseScoreByFort(map, x, y - 1, move, tmpBaseScoreMap);
			setBaseScoreByFort(map, x - 1, y, move, tmpBaseScoreMap);
			setBaseScoreByFort(map, x + 1, y, move, tmpBaseScoreMap);
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