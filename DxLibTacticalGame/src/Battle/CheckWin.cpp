#include "CheckWin.h"

namespace Battle {

	/**
	 * @fn
	 * ターン制限による勝敗条件追加
	 * @param (turnNum) ターン制限
	 * @param (isPlayerWinOverLimit) ターン制限を超えたときにプレイヤー側が勝利の場合: true
	*/
	void CheckWin::setLimitTurn(int limitTurn, bool isPlayerWinOverLimit)
	{
		limitTurn_ = limitTurn;
		isPlayerWinOverLimit_ = isPlayerWinOverLimit;
	}

	/**
	 * @fn
	 * 防衛地点による勝敗条件追加
	 * @param (isPlayer) プレイヤー側の防衛地点の場合: true
	*/
	void CheckWin::setBaseDefense(bool isPlayer)
	{
		if (isPlayer)
		{
			isPlayerBaseDefense_ = true;
		}
		else
		{
			isEnemyBaseDefense_ = true;
		}
	}

	/**
	 * @fn
	 * 勝敗判定
	 * @param (turnNum) 経過ターン
	*/
	void CheckWin::checkWin(int turnNum)
	{
		if (limitTurn_ == 0)
		{
			return;
		}

		// ターンオーバー
		if (turnNum > limitTurn_)
		{
			if (isPlayerWinOverLimit_)
			{
				winner_ = Winner::PLAYER;
			}
			else
			{
				winner_ = Winner::ENEMY;
			}
		}
	}

	/**
	 * @fn
	 * 勝敗判定
	 * @param (map) Mapのスマートポインタ
	*/
	void CheckWin::checkWin(shared_ptr<Map> map)
	{
		winner_ = checkWipeOut(map); // ユニット全滅判定

		if (winner_ != Winner::UNDECIDED)
		{
			return;
		}

		if (isPlayerBaseDefense_) // プレイヤー側防衛拠点判定
		{
			pair<int, int> fortPos = map->getPlayerFortMass();
			shared_ptr<Unit> unit = map->getUnit(fortPos.first, fortPos.second);

			if (unit && unit->isEnemy())
			{
				winner_ = Winner::ENEMY;
				return;
			}
		}

		if (isEnemyBaseDefense_) // 敵側防衛拠点判定
		{
			pair<int, int> fortPos = map->getEnemyFortMass();
			shared_ptr<Unit> unit = map->getUnit(fortPos.first, fortPos.second);

			if (unit && !unit->isEnemy())
			{
				winner_ = Winner::PLAYER;
			}
		}
	}

	/**
	 * @fn
	 * ユニット全滅判定
	 * @return 勝敗判定結果
	*/
	int CheckWin::checkWipeOut(shared_ptr<Map> map)
	{
		bool existEnemyUnit = false;
		bool existPlayerUnit = false;

		// どちらかのユニットが全ていなくなっているかチェック
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			if (itr->second->isEnemy())
			{
				existEnemyUnit = true;
			}
			else
			{
				existPlayerUnit = true;
			}

			if (existEnemyUnit && existPlayerUnit)
			{
				return Winner::UNDECIDED;
			}
		}

		if (!existPlayerUnit)
		{
			return Winner::ENEMY;
		}
		else if (!existEnemyUnit)
		{
			return Winner::PLAYER;
		}

		return Winner::UNDECIDED;
	}
}